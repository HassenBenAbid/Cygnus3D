#include "MeshLoader.h"
#include <glm/glm.hpp>
#include <iostream>

#include "../Components/Scene/Node.h"
#include "../Components/Rendering/Material.h"
#include "../Components/Rendering/Mesh.h"
#include "../Components/Animation/Joint.h"
#include "../Components/Scene/Animator.h"

namespace Cygnus3D {

	std::map<std::string, Joint*> MeshLoader::m_currentJoints;

	Node* MeshLoader::loadMesh(std::string path, bool flipUv) {

		Assimp::Importer importer;

		unsigned int flipState = 0;
		if (flipUv) flipState = aiProcess_FlipUVs;

		aiScene const *aScene = importer.ReadFile(path, aiProcess_Triangulate | flipState | aiProcess_JoinIdenticalVertices | aiProcess_ConvertToLeftHanded);

		if (!aScene || aScene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !aScene->mRootNode) {
			std::cout << "ASSIMP FAILED TO LOAD SCENE!" << std::endl;
			return nullptr;
		}

		std::string directory = path.substr(0, path.find_last_of("/")) + "/";

		std::vector<Material*> allMaterials;

		for (int i = 0; i < aScene->mNumMaterials; i++) {
			allMaterials.push_back(parseMaterial(aScene->mMaterials[i], aScene, directory));
		}

		Animator *anim = new Animator();
		Node *node = MeshLoader::processNode(aScene->mRootNode, aScene, allMaterials, anim);

		if (aScene->HasAnimations()) {
			anim->m_animationCount = aScene->mNumAnimations;
			anim->m_inverseGlobal = castGlm(aScene->mRootNode->mTransformation, false);
			anim->m_inverseGlobal = glm::inverse(anim->m_inverseGlobal);

			node->setAnimator(anim);

		}

		return node;

	}

	Node* MeshLoader::processNode(aiNode *aNode, aiScene const *aScene, std::vector<Material*> allMaterials, Animator *anim) {

		Node *motherNode = new Node();

		//std::cout << aNode->mNumMeshes << "||" << aScene->mNumMeshes << std::endl;

		for (int i = 0; i < aNode->mNumMeshes; i++) {

			aiMesh *aMesh = aScene->mMeshes[aNode->mMeshes[i]];
			Mesh *mesh = MeshLoader::parseMesh(aMesh, aScene);

			if (aMesh->HasBones()) {
				loadBones(mesh, aMesh, anim);
				anim->m_rootJoint = createJoints(aScene->mRootNode);
				loadBonesAnimation(aScene);
				m_currentJoints.clear();
			}

			mesh->generate();

			Material *material = allMaterials[aMesh->mMaterialIndex];

			if (aNode->mNumMeshes == 1) {
				motherNode->setMesh(mesh);
				motherNode->setMaterial(material);
			}
			else {
				Node *childNode = new Node(mesh, material);
				motherNode->addChild(childNode);
			}
		}

		for (int i = 0; i < aNode->mNumChildren; i++) {
			motherNode->addChild(MeshLoader::processNode(aNode->mChildren[i], aScene, allMaterials, anim));
		}

		return motherNode;
	}

	Mesh* MeshLoader::parseMesh(aiMesh *aMesh, aiScene const *aScene) {

		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> texCoords;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> indices;

		positions.resize(aMesh->mNumVertices);
		normals.resize(aMesh->mNumVertices);
		if (aMesh->mNumUVComponents > 0) {
			texCoords.resize(aMesh->mNumVertices);
		}

		indices.resize(aMesh->mNumFaces * 3);

		for (int i = 0; i < aMesh->mNumVertices; i++) {
			positions[i] = glm::vec3(aMesh->mVertices[i].x, aMesh->mVertices[i].y, aMesh->mVertices[i].z);
			if (aMesh->mNormals) normals[i] = glm::vec3(aMesh->mNormals[i].x, aMesh->mNormals[i].y, aMesh->mNormals[i].z);
			if (aMesh->mTextureCoords[0]) {
				texCoords[i] = glm::vec2(aMesh->mTextureCoords[0][i].x, aMesh->mTextureCoords[0][i].y);
			}
		}

		for (int i = 0; i < aMesh->mNumFaces; i++) {
			for (int j = 0; j < 3; j++) {
				indices[i * 3 + j] = aMesh->mFaces[i].mIndices[j];
			}
		}

		Mesh *mesh = new Mesh(positions, normals, indices);
		mesh->setTexCoords(texCoords);

		return mesh;

	}

	Material* MeshLoader::parseMaterial(aiMaterial *aMaterial, aiScene const *aScene, std::string path) {

		Material *material = new Material();

		if (aMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {

			aiString file;
			aMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);

			std::cout << MeshLoader::parseDirectory(path, file).c_str() << std::endl;

			Texture *texture = new Texture(MeshLoader::parseDirectory(path, file).c_str());
			if (texture) {
				material->setTexture(texture);
			}

			if (aMaterial->GetTextureCount(aiTextureType_SPECULAR) > 0) {

				aiString file;
				aMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);

				Texture *texture = new Texture(MeshLoader::parseDirectory(path, file).c_str());
				if (texture) {
					material->setSpecularMap(texture);
				}

			}
		}

		material->setShininess(20.0f);

		return material;

	}

	void MeshLoader::loadBones(Mesh *mesh, aiMesh *aMesh, Animator *anim) {

		unsigned int boneIndex = 0;

		mesh->m_bonesWeight.resize(aMesh->mNumVertices);
		mesh->m_bonesID.resize(aMesh->mNumVertices);

		for (int i = 0; i < aMesh->mNumBones; i++) {
			
			std::string BoneName = aMesh->mBones[i]->mName.data;
			unsigned int currentBoneIndex = 0;

			if (anim->m_boneMapping.find(BoneName) == anim->m_boneMapping.end()) {

				BoneInfo *newInfo = new BoneInfo();
				newInfo->boneOffset = castGlm(aMesh->mBones[i]->mOffsetMatrix, true);
				newInfo->finalTransformation = glm::mat4(1.f);

				anim->m_boneInfo.push_back(newInfo);
				anim->m_boneMapping[BoneName] = boneIndex;

				currentBoneIndex = boneIndex; 
				boneIndex++;
			}
			else {
				boneIndex = anim->m_boneMapping[BoneName];
				currentBoneIndex = boneIndex;
			}

			for (int j = 0; j < aMesh->mBones[i]->mNumWeights ; j++) {
				unsigned int vertexID = aMesh->mBones[i]->mWeights[j].mVertexId;
				float weight = aMesh->mBones[i]->mWeights[j].mWeight;
				addBoneData(mesh, currentBoneIndex, weight, vertexID);
			}

		}
	}

	//Animation::

	Joint* MeshLoader::createJoints(aiNode *aNode) {

		Joint *currentJoint = new Joint(aNode->mName.data);
		m_currentJoints.insert(std::pair<std::string, Joint*>(aNode->mName.data, currentJoint));

		for (int i = 0; i < aNode->mNumChildren; i++) {
			currentJoint->addChild(createJoints(aNode->mChildren[i]));
		}

		return currentJoint;
	}

	void MeshLoader::loadBonesAnimation(const aiScene *aScene) {

		for (int i = 0; i < aScene->mNumAnimations; i++) {
			aiAnimation *anim = aScene->mAnimations[i];
			for (int j = 0; j < anim->mNumChannels; j++) {
				aiNodeAnim *aNode = anim->mChannels[j];
				jointTransform *currentTransform = new jointTransform();
				for (int k = 0; k < aNode->mNumPositionKeys; k++) {
					currentTransform->positions.push_back(castGlm(aNode->mPositionKeys[k].mValue));
					currentTransform->rotations.push_back(castGlm(aNode->mRotationKeys[k].mValue));
					currentTransform->timeDuration.push_back(aNode->mPositionKeys[k].mTime);
				}

				m_currentJoints.find(aNode->mNodeName.data)->second->addTransform(currentTransform);
			}
		}
	}

	//Utils::

	glm::quat MeshLoader::castGlm(aiQuaternion quat) {
		glm::quat currentQuat;

		currentQuat.x = quat.x;
		currentQuat.y = quat.y;
		currentQuat.z = quat.z;
		currentQuat.w = quat.w;

		return currentQuat;
	}

	glm::vec3 MeshLoader::castGlm(aiVector3D vector) {

		glm::vec3 vec;

		vec.x = vector.x;
		vec.y = vector.y;
		vec.z = vector.z;

		return vec;
	}

	glm::mat4 MeshLoader::castGlm(aiMatrix4x4 matrix, bool inverse) {

		glm::mat4 newMatrix;

		if (!inverse) {

			newMatrix[0][0] = matrix[0][0];
			newMatrix[0][1] = matrix[0][1];
			newMatrix[0][2] = matrix[0][2];
			newMatrix[0][3] = matrix[0][3];

			newMatrix[1][0] = matrix[1][0];
			newMatrix[1][1] = matrix[1][1];
			newMatrix[1][2] = matrix[1][2];
			newMatrix[1][3] = matrix[1][3];

			newMatrix[2][0] = matrix[2][0];
			newMatrix[2][1] = matrix[2][1];
			newMatrix[2][2] = matrix[2][2];
			newMatrix[2][3] = matrix[2][3];

			newMatrix[3][0] = matrix[3][0];
			newMatrix[3][1] = matrix[3][1];
			newMatrix[3][2] = matrix[3][2];
			newMatrix[3][3] = matrix[3][3];
		}
		else {

			newMatrix[0][0] = matrix[0][0];
			newMatrix[0][1] = matrix[1][0];
			newMatrix[0][2] = matrix[2][0];
			newMatrix[0][3] = matrix[3][0];

			newMatrix[1][0] = matrix[0][1];
			newMatrix[1][1] = matrix[1][1];
			newMatrix[1][2] = matrix[2][1];
			newMatrix[1][3] = matrix[3][1];

			newMatrix[2][0] = matrix[0][2];
			newMatrix[2][1] = matrix[1][2];
			newMatrix[2][2] = matrix[2][2];
			newMatrix[2][3] = matrix[3][2];

			newMatrix[3][0] = matrix[0][3];
			newMatrix[3][1] = matrix[1][3];
			newMatrix[3][2] = matrix[2][3];
			newMatrix[3][3] = matrix[3][3];
		}

		return newMatrix;
	}

	void MeshLoader::addBoneData(Mesh *mesh, unsigned int boneID, float weight, unsigned int vertexID) {

		for (int i = 0; i < 3; i++) {
			if (mesh->m_bonesWeight[vertexID][i] == 0) {
				mesh->m_bonesWeight[vertexID][i] = weight;
				mesh->m_bonesID[vertexID][i] = boneID;
				if (i == 3) glm::normalize(mesh->m_bonesWeight[vertexID]);
				break;
			}
		}

	}

	std::string MeshLoader::parseDirectory(std::string directory, aiString file) {

		directory = directory + std::string(file.C_Str());

		return directory;

	}

}