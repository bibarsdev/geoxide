
#ifndef __GX_SKELETAL_ANIMATION_H__
#define __GX_SKELETAL_ANIMATION_H__

namespace Geoxide {
	
	struct Bone
	{
		Matrix localMatrix;
		Matrix invModelMatrix;
		uint32_t parentIndex;
	};

	class SkeletalAnimation
	{
	public:
		SkeletalAnimation() : mDuration(0) {}

		const std::vector<Matrix>& getFrame(float timeStamp) const;
		float getDuration() const { return mDuration; }

		void calculateAndPushFrame(const std::vector<Bone>& bones, const std::vector<Matrix>& matrices);

	private:
		std::vector<std::vector<Matrix>> mFrames;
		float mDuration;

		friend class ResourceManager;
	};

	class SkeletalAnimationState
	{
	public:
		SkeletalAnimationState() : mAnimation(0), mCyclic(false) {}
		SkeletalAnimationState(SkeletalAnimation* anim, bool cyclic) : mAnimation(anim), mCyclic(cyclic) {}

		bool hasAnimation() { return mAnimation != 0; }

		const std::vector<Matrix>& getCurrentFrame() const { return mAnimation->getFrame(mTimeStamp); }

		void setAnimation(SkeletalAnimation* anim) { mAnimation = anim; }
		void setCyclic(bool cyclic) { mCyclic = cyclic; }

		void addTime(float time);
		void reset() { mTimeStamp = 0; }

	private:
		SkeletalAnimation* mAnimation;
		float mTimeStamp;
		bool mCyclic;
	};

	class Skeleton
	{
	public:
		Skeleton() {}

		const SkeletalAnimation& getAnimationByIndex(uint32_t i) const { return mAnimations.begin().operator->()[i].second; }
		const SkeletalAnimation& getAnimationByName(const std::string& name) const { return mAnimations.at(name); }

		uint32_t getNumAnimations() const { return mAnimations.size(); }

	private:
		std::vector<Bone> mBones;
		std::unordered_map<std::string, SkeletalAnimation> mAnimations;

		friend class ResourceManager;
	};
}

#endif
