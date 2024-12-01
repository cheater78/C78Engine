#pragma once

#include <C78EPCH.h>

#include <C78E/Renderer/Assets/Mesh/MeshUtils.h>
#include <C78E/Renderer/Assets/Mesh/Mesh.h>

namespace C78E {
	/*
	class BVH {
	private:

		using TriangleIndex = size_t;
		using TriangleCount = size_t;
		using BoundingBoxIndex = size_t;
		using NodeIndex = size_t;

		// Node of the binary tree that represents the BoundingVolumeHirarchy
		struct Node {
			BoundingBox boundingBox{};
			TriangleIndex triangleIndex = 0;
			TriangleCount triangleCount = 1; // Every BoundingBox needs atleast 1 Triangle!
			NodeIndex childIndex = 0;
		};

	public:
		BVH() = delete;
		BVH(C78E::Ref<Mesh> mesh, uint32_t maxDepth = 32) : m_Mesh(mesh), m_MaxDepth(maxDepth) { build(); }
		BVH(const BVH& other) = delete;
		~BVH() = default;

	private:
		void build() {
			m_Triangles = m_Mesh->getTriangles();

			m_Nodes.push_back(
				Node{						// root Node
					BoundingBox(*m_Mesh),		// root BoundingBox
					0,							// TriangleIndex
					m_Triangles.size(),			// TriangleCount
					0							// Child NodeIndex
				}
			);

			splitNode(0);
		}

		void splitNode(NodeIndex parentNodeIndex, uint32_t depth = 0) {
			if (depth >= m_MaxDepth) return;
			Node& parent = m_Nodes[parentNodeIndex];

			// determine which axis to split and at which value
			const uint32_t splitAxisIndex = [parent](void) -> uint32_t {
				const glm::vec3 center = parent.boundingBox.center();
				return center.x > glm::max(center.y, center.z) ? 0 : center.y > center.z ? 1 : 2;
			}();
			const float splitAxisValue = parent.boundingBox.center()[splitAxisIndex];
			
			parent.childIndex = m_Nodes.size();
			// create child A and B
			m_Nodes.reserve(m_Nodes.size() + 2);
			m_Nodes.push_back(Node()); 
			m_Nodes.push_back(Node());
			const NodeIndex childAIndex = parent.childIndex + 0;
			const NodeIndex childBIndex = parent.childIndex + 1;
			Node& childA = m_Nodes[childAIndex];
			Node& childB = m_Nodes[childBIndex];


			for (TriangleIndex i = parent.triangleIndex; i < parent.triangleIndex + parent.triangleCount; i++) {
				const Geometry::Triangle currentTriangle = m_Triangles[i]; // const copy of the current Triangle that should be sorted

				const bool isInChildA = currentTriangle.center()[splitAxisIndex] < splitAxisValue;
				Node& enclosingChild = isInChildA ? childA : childB;

				enclosingChild.boundingBox.growToInclude(currentTriangle);
				enclosingChild.triangleCount++;

				if (isInChildA) {
					// sort childA enclosed Triangles to the front of the Buffer
					uint32_t requiredIndex = enclosingChild.triangleIndex + enclosingChild.triangleCount - 1;
					m_Triangles[i] = m_Triangles[requiredIndex]; // place the childB Triangle at the current Index
					m_Triangles[requiredIndex] = currentTriangle; // place the current Triangle at the required Index
					childB.triangleIndex++; // childA Triangle uses the first childB Triangle space -> correct for that
				}
			}

			splitNode(childAIndex, depth + 1);
			splitNode(childBIndex, depth + 1);
		}

	private:
		Ref<Mesh> m_Mesh;
		uint32_t m_MaxDepth;
		std::vector<Geometry::Triangle> m_Triangles;
		std::vector<Node> m_Nodes;

	};
	*/
}
