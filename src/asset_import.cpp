#include "asset_import.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

#include "types.h"
#include "assimp_util.h"

namespace sws {
    void import_mesh(const char *path, sws::MeshGroup &assets) {
        const aiScene *scene = aiImportFile(path, aiProcess_Triangulate);

        if (scene == nullptr || !scene->HasMeshes()) {
            printf("Unable to load basic_scene.glb\n");
            exit(255);
        } else {
            printf("Scene loaded.\n");
        }

        for (i32 m = 0; m < scene->mNumMeshes; m++) {
            sws::Mesh &mesh = assets.meshes.emplace_back();
            const auto &ai_mesh = scene->mMeshes[m];

            for (u32 f = 0; f < ai_mesh->mNumFaces; f++) {
                const auto &ai_face = ai_mesh->mFaces[f];
                assert(ai_face.mNumIndices == 3);
                const u32 indices[3] = {ai_face.mIndices[0], ai_face.mIndices[1], ai_face.mIndices[2]};
                for (const auto idx: indices) {
                    assert(idx < ai_mesh->mNumVertices);
                    const auto &ai_vec = ai_mesh->mVertices[idx];
                    mesh.positions.emplace_back(ai_vec.x, ai_vec.y, ai_vec.z);

                    const auto &ai_norm = ai_mesh->mNormals[idx];
                    mesh.normals.emplace_back(ai_norm.x, ai_norm.y, ai_norm.z);
                }
            }
        }

        for (i32 i = 0; i < scene->mNumMaterials; i++) {
            aiMaterial *ai_material = scene->mMaterials[i];
            aiString name;
            ai_material->Get(AI_MATKEY_NAME, name);
            assets.materials.emplace_back(sws::get_material(name.data));
        }

        printf("  Meshes loaded: %d\n", assets.meshes.size());

        // TODO: Make a proper scene graph
        const auto root = scene->mRootNode;
        // TODO: Handle root and make this recursive
        for (i32 c = 0; c < root->mNumChildren; c++) {
            const auto &child = root->mChildren[c];

            if (child->mNumMeshes == 0) {
                continue;
            }

            if (child->mNumChildren > 0) {
                printf("Child with children: %s", child->mName.data);
            }

            auto &node = assets.nodes.emplace_back();
            node.transform = ConvertMatrixToGLMFormat(child->mTransformation);
            for (i32 m = 0; m < child->mNumMeshes; m++) {
                node.mesh_indices.push_back(child->mMeshes[m]);
                node.material_indices.push_back(scene->mMeshes[child->mMeshes[m]]->mMaterialIndex);
            }

        }

        printf("Converted from assimp to local representation.\n");
        aiReleaseImport(scene);
    }


}
