
#ifndef __GX_ENTITY_REGISTRY_H__
#define __GX_ENTITY_REGISTRY_H__

namespace Geoxide {

	using entity_t = size_t;

	class EntityRegistry
	{
	private:
		using ComponentBuffer = std::pair<std::vector<uint8_t>, std::vector<entity_t>>;

	public:
		EntityRegistry()
		{
		}

		~EntityRegistry()
		{
		}

		entity_t newEntity()
		{
			entity_t entity = mEntities.size();
			mEntities.push_back(0);
			return entity;
		}
		
		template <typename T, typename... Args>
		T& addComponent(entity_t entity, Args&& ... args)
		{
			auto& buffer = mComBuffers[typeid(T).hash_code()];

			buffer.second.push_back(entity);

			size_t temp = buffer.first.size();

			buffer.first.resize(temp + sizeof(T));
			return (T&)buffer.first[temp] = T(std::forward<Args>(args)...);
		}

		template <typename T>
		T* getComponent(entity_t entity)
		{
			auto& buffer = mComBuffers[typeid(T).hash_code()];

			auto& iter = std::find(buffer.second.begin(), buffer.second.end(), entity);

			if (iter != buffer.second.end())
			{
				size_t index = std::distance(buffer.second.begin(), iter);

				return (T*)buffer.first.data() + index;
			}

			return 0;
		}

		template <typename T>
		View<T> getComponents()
		{
			auto& buffer = mComBuffers[typeid(T).hash_code()];

			T* begin = (T*)buffer.first.begin()._Ptr;
			T* end = (T*)buffer.first.end()._Ptr;

			return View<T>(begin, end);
		}

		template <typename T>
		View<entity_t> getEntities()
		{
			auto& buffer = mComBuffers[typeid(T).hash_code()];

			entity_t* begin = (entity_t*)buffer.second.begin()._Ptr;
			entity_t* end = (entity_t*)buffer.second.end()._Ptr;

			return View<entity_t>(begin, end);
		}

	private:
		std::unordered_map<hash_t, ComponentBuffer> mComBuffers;
		std::vector<char> mEntities;
	};

	class Entity
	{
	public:
		Entity(EntityRegistry* registry, entity_t entity) : mRegistry(registry), mEntity(entity) {}
		~Entity() {}

		template <typename T, typename... Args>
		T& addComponent(Args&& ... args)
		{
			return mRegistry->addComponent<T>(mEntity, std::forward<Args>(args)...);
		}

		template <typename T>
		T* getComponent()
		{
			return mRegistry->getComponent<T>(mEntity);
		}

	private:
		EntityRegistry* mRegistry;
		entity_t mEntity;
	};

}

#endif
