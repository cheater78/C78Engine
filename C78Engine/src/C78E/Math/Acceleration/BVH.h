#pragma once
#include <C78E/Core/Log/SmartLog.h>
#include <C78E/Math/Core/Core.h>
#include <C78E/Math/Geometry/Geometry.h>

namespace C78E::Math {
	/*
	template<Dimension dim>
	class BoundingVolumeHirarchy {
	public:
		using NodeIndex = size_t;
		using Item = size_t;
		using ItemIndex = size_t;
		using ItemCount = size_t;

		using ItemVolumeResolver = std::function<AABB<dim>(ItemIndex)>;

	public:
		BoundingVolumeHirarchy() = delete;
		BoundingVolumeHirarchy(ItemCount itemCount, ItemVolumeResolver resolver, uint32_t maxDepth = 32)
			: m_MaxDepth(maxDepth), m_Resolver(resolver) {
			C78E_CORE_ASSERT(m_Resolver, "BoundingVolumeHirarchy: ItemPositionResolver provided was null!");
			rebuild(itemCount);
		}
		BoundingVolumeHirarchy(BoundingVolumeHirarchy&) = delete;
		~BoundingVolumeHirarchy() = default;

		void rebuild(ItemCount itemCount) {
			m_Nodes.clear();
			m_Items.clear();
			m_Nodes.reserve(static_cast<size_t>(glm::log2(static_cast<float>(itemCount))));
			m_Items.reserve(itemCount);

			m_Nodes.emplace_back(); // Root Node, init AABB with whole mesh

			splitNode(0);
		}

	private:
		void splitNode(NodeIndex parentNodeIndex, uint32_t depth = 0) {
			if(depth >= m_MaxDepth) return;
			Node& parent = m_Nodes[parentNodeIndex];

			// determine which axis to split and at which value
			const uint32_t splitAxisIndex = [parent](void) -> uint32_t {
				const glm::vec3 center = parent.center();
				return center.x > glm::max(center.y, center.z) ? 0 : center.y > center.z ? 1 : 2;
				}();
			const float splitAxisValue = parent.center()[splitAxisIndex];

			parent.childIndex = m_Nodes.size();
			// create child A and B
			m_Nodes.reserve(m_Nodes.size() + 2);
			m_Nodes.emplace_back();
			m_Nodes.emplace_back();
			const NodeIndex childAIndex = parent.childIndex + 0;
			const NodeIndex childBIndex = parent.childIndex + 1;
			Node& childA = m_Nodes[childAIndex];
			Node& childB = m_Nodes[childBIndex];


			for(ItemIndex i = parent.baseItemIndex; i < parent.baseItemIndex + parent.itemCount; i++) {
				// the current Face that should be sorted
				const ItemIndex currentItemIndex = m_Items[i];
				const AABB currentItemBB = m_Resolver(currentItemIndex);

				const bool isInChildA = currentItemBB.center()[splitAxisIndex] < splitAxisValue;
				Node& enclosingChild = isInChildA ? childA : childB;

				C78E_CORE_ASSERT(false, "growToInclude requieres the Items dimensions or any metric to include the actual Item");
				enclosingChild.growToInclude(currentItemBB);
				enclosingChild.itemCount++;

				if(isInChildA) {
					// sort childA enclosed Faces to the front of the Buffer
					ItemIndex requiredIndex = enclosingChild.baseItemIndex + enclosingChild.itemCount - 1;
					m_Items[i] = m_Items[requiredIndex]; // place the childB Face at the current Index
					m_Items[requiredIndex] = currentItemIndex; // place the current Face at the required Index
					childB.baseItemIndex++; // childA Face uses the first childB Face space -> correct for that
				}
			}

			splitNode(childAIndex, depth + 1);
			splitNode(childBIndex, depth + 1);
		}

		// Node of the binary tree that represents the BoundingVolumeHirarchy
		struct Node : public AABB<dim> {
			NodeIndex childIndex = 0; // first ChildIndex, second is implicit +1
			ItemIndex baseItemIndex = 0; // Index of the first item, itemCount Items follow
			ItemCount itemCount = 1; // atleast 1 Item
		};

		uint32_t m_MaxDepth;
		std::vector<Node> m_Nodes;
		std::vector<Item> m_Items;
		ItemVolumeResolver m_Resolver;
	};
	*/
}