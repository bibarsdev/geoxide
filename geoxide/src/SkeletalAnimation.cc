
#include "Geoxide/SkeletalAnimation.h"

namespace Geoxide {

	bool AnimationTrack::insertKeyFrame(float time, const KeyFrame& frame)
	{
		auto& pair = mKeyFrames.insert({ time, frame });

		return pair.second;
	}

	bool AnimationTrack::emplaceKeyFrame(float time, cfloat3 position, cquat quaternion, cfloat3 scaling)
	{
		auto& pair = mKeyFrames.try_emplace(time, position, quaternion, scaling);

		return pair.second;
	}

	float4x4 AnimationTrack::computeMatrix(float time)
	{
		auto& lower_bound = mKeyFrames.lower_bound(time);
		
		float4x4 matrix = 1;

		if (lower_bound != mKeyFrames.end())
		{
			float4x4 translation = 1;
			float4x4 rotation = 1;
			float4x4 scaling = 1;

			if (lower_bound->first == time)
			{
				auto& a = lower_bound->second;

				float4x4::translation(a.position, translation);
				float4x4::rotation_quaternion(a.quaternion, rotation);
				float4x4::scaling(a.scaling, scaling);
			}
			else
			{
				auto& b_pair = *lower_bound;

				std::pair<float, KeyFrame> a_pair;

				if (lower_bound == mKeyFrames.begin())
				{
					a_pair = *(--mKeyFrames.end());
				}
				else
				{
					a_pair = *(--lower_bound);
				}

				float a_time = a_pair.first;
				float b_time = b_pair.first;

				auto& a = a_pair.second;
				auto& b = b_pair.second;

				float t = (time - a_time) / (b_time - a_time);

				float4x4::translation(a.position.interpolate(b.position, t), translation);
				float4x4::rotation_quaternion(a.quaternion.interpolate(b.quaternion, t), rotation);
				float4x4::scaling(a.scaling.interpolate(b.scaling, t), scaling);
			}

			matrix = translation * rotation * scaling;
		}

		return matrix;
	}

	void SkeletalAnimation::computeMatrices(float time, float4x4* outMatrices)
	{
		auto& bones = mSkeleton->getBones();

		std::vector<float4x4> animMatrices(bones.size());
		std::vector<float4x4> animModelMatrices(bones.size());

		for (uint32_t i = 0; i < bones.size(); i++)
		{
			animMatrices[i] = mTracks[i].computeMatrix(time);
		}

		animModelMatrices[0] = animMatrices[0];

		for (uint32_t i = 1; i < bones.size(); i++)
		{
			uint32_t parent = bones[i].parentIndex;
			animModelMatrices[i] = animModelMatrices[parent] * animMatrices[i];
		}

		for (uint32_t i = 0; i < bones.size(); i++)
		{
			outMatrices[i] = animModelMatrices[i] * bones[i].invModelMatrix;
		}

	}

	float4x4* SkeletalAnimationState::getCurrentFrameMatrices()
	{
		if (mCachedTimeStamp != mTimeStamp)
		{
			mAnimation->computeMatrices(mTimeStamp, mCachedMatrices.get());
			mCachedTimeStamp = mTimeStamp;
		}

		return mCachedMatrices.get();
	}

	void SkeletalAnimationState::addTime(float time)
	{
		float temp = mTimeStamp + time;

		if (mCyclic)
		{
			mTimeStamp = fmod(temp, mAnimation->getDuration());
		}
		else
		{
			mTimeStamp = temp;
		}
	}
}
