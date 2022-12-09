
#include "Geoxide/SkeletalAnimation.h"

namespace Geoxide {

	const std::vector<Matrix>& SkeletalAnimation::getFrame(float timeStamp) const
	{
		uint32_t frameIndex = (uint32_t)(mDuration / timeStamp);

		return mFrames[frameIndex];
	}

	void SkeletalAnimation::calculateAndPushFrame(const std::vector<Bone>& bones, const std::vector<Matrix>& matrices)
	{
		Matrix* localMatrices = new Matrix[bones.size()];
		Matrix* modelMatrices = new Matrix[bones.size()];
		auto& frameMatrices = mFrames.back();

		for (uint32_t i = 0; i < bones.size(); i++)
		{
			localMatrices[i] = MatrixMultiply(bones[i].localMatrix, matrices[i]);
		}

		modelMatrices[0] = localMatrices[0];

		for (uint32_t i = 1; i < bones.size(); i++)
		{
			uint32_t parent = bones[i].parentIndex;
			modelMatrices[i] = MatrixMultiply(modelMatrices[parent], localMatrices[i]);
		}

		for (uint32_t i = 0; i < bones.size(); i++)
		{
			frameMatrices[i] = MatrixMultiply(modelMatrices[i], bones[i].invModelMatrix);
		}

		delete localMatrices;
		delete modelMatrices;
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
			if (temp < mAnimation->getDuration())
			{
				mTimeStamp = temp;
			}
			else
			{
				mTimeStamp = mAnimation->getDuration();
			}
		}
	}
}
