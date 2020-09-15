#pragma once 

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <map>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/matrix.hpp>
#include <glm/gtx/quaternion.hpp>


namespace Cygnus3D {

	class Mesh;
	class Node;
	class Joint;
	class Animator;
	class Material;

	class MeshLoader {

	private:

		static std::map<std::string, Joint*> m_currentJoints;

		static Node* processNode(aiNode *aNode, aiScene const *aScene, std::vector<Material*> allMaterials, Animator *anim = nullptr);
		static Mesh* parseMesh(aiMesh* aMesh, aiScene const *aScene);
		static Material* parseMaterial(aiMaterial *aMaterial, aiScene const *aScene, std::string path);
		static std::string parseDirectory(std::string directory, aiString file);

		static void loadBones(Mesh *mesh, aiMesh *aMesh, Animator *anim);
		static Joint* createJoints(aiNode *aNode);
		static void loadBonesAnimation(const aiScene *aScene);

		static glm::mat4 castGlm(aiMatrix4x4 matrix, bool inverse);
		static glm::vec3 castGlm(aiVector3D vector);
		static glm::quat castGlm(aiQuaternion quat);

		static void addBoneData(Mesh *mesh, unsigned int boneID, float weight, unsigned int vertexID);

	public:
		
		static Node* loadMesh(std::string path, bool flipUv = true);

	};
}