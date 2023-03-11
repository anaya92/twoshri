// bandaid: hacky solutions file
// and testing!!!!
#include <stdio.h>
#include <stdbool.h>
#include <stb_image.h>

#include "R/renderer.h"

R_ImageSpec Ba_LoadImage(const char* file, bool vflip)
{
    stbi_set_flip_vertically_on_load(vflip);
    
    int w, h, channelCount;
    unsigned char* data = stbi_load(file, &w, &h, &channelCount, 0);

    if (data)
    {
        return (R_ImageSpec){data, w, h, channelCount};
    }
    else
    {
        printf("[bandaid] failed to load texture %s\n", file);
        return (R_ImageSpec){NULL, 0, 0, 0};
    }
}

float* triangleStripTestVertices;
void Ba_GenerateTriangleStripTestMesh()
{

}

void Ba_LoadMap()
{
    static const char* filepath = "data/map/dreamland.toml";
}

// #include <cgltf.h>

// void Ba_GltfTester()
// {
//     void* buf; /* Pointer to glb or gltf file data */
//     size_t size; /* Size of the file data */

//     cgltf_options options = {0};
//     cgltf_data* data = NULL;
//     cgltf_result result = cgltf_parse(&options, buf, size, &data);
//     if (result == cgltf_result_success)
//     {
//         /* TODO make awesome stuff */
//         cgltf_free(data);
//     }
// }