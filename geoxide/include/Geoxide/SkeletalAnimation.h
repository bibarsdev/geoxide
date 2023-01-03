
#ifndef __GX_SKELETAL_ANIMATION_H__
#define __GX_SKELETAL_ANIMATION_H__

#include <map>

namespace Geoxide {
	
	class Skeleton;

	struct Bone
	{
		uint32_t parentIndex;
		float4x4 localMatrix;
		float4x4 invModelMatrix;
	};

	struct KeyFrame
	{
		KeyFrame() {}
		KeyFrame(float3 position, quat quaternion, float3 scaling)
			: position(position), quaternion(quaternion), scaling(scaling) {}

		float3 position;
		quat quaternion;
		float3 scaling;
	};

	class AnimationTrack
	{
	public:
		AnimationTrack() {}
		~AnimationTrack() {}

		bool insertKeyFrame(float time, const KeyFrame& frame);
		bool emplaceKeyFrame(float time, cfloat3 position, cquat quaternion, cfloat3 scaling);

		float4x4 computeMatrix(float time);

	private:
		std::map<float, KeyFrame> mKeyFrames;

		friend class Application;
	};

	class SkeletalAnimation
	{
	public:
		SkeletalAnimation() : mDuration(0) {}
		SkeletalAnimation(float duration) : mDuration(duration) {}

		float getDuration() const { return mDuration; }
		uint32_t getNumTracks() const { return mTracks.size(); }

		void computeMatrices(float time, float4x4* outMatrices);

	private:
		std::vector<AnimationTrack> mTracks;
		float mDuration;

		Skeleton* mSkeleton;

		friend class Application;
	};

	class SkeletalAnimationState
	{
	public:
		SkeletalAnimationState() : mAnimation(0), mCyclic(false), mTimeStamp(0), mCachedTimeStamp(NAN) {}
		SkeletalAnimationState(SkeletalAnimation* anim, bool cyclic) : mAnimation(anim), mCyclic(cyclic), mTimeStamp(0), mCachedTimeStamp(NAN) {}

		bool hasAnimation() { return mAnimation != 0; }

		float4x4* getCurrentFrameMatrices();

		void setAnimation(SkeletalAnimation* anim) 
		{
			mAnimation = anim;
			mCachedMatrices.reset(new float4x4[ anim->getNumTracks() ]);
			mCachedTimeStamp = NAN;
			mTimeStamp = 0;
		}

		void setCyclic(bool cyclic) { mCyclic = cyclic; }

		void addTime(float time);
		void reset() { mTimeStamp = 0; }

	private:
		SkeletalAnimation* mAnimation;
		Local<float4x4> mCachedMatrices;

		float mTimeStamp;
		float mCachedTimeStamp;

		bool mCyclic;
	};

	class Skeleton
	{
	public:
		Skeleton() {}

		SkeletalAnimation& getAnimationByIndex(uint32_t i) { return mAnimations.begin().operator->()[i].second; }
		SkeletalAnimation& getAnimationByName(const std::string& name) { return mAnimations.at(name); }
		uint32_t getNumAnimations() const { return mAnimations.size(); }

		const std::vector<Bone>& getBones() const { return mBones; }

	private:
		std::vector<Bone> mBones;
		std::unordered_map<std::string, SkeletalAnimation> mAnimations;

		friend class Application;
	};
}

#endif
