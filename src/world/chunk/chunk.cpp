#include "world/chunk/chunk.hpp"
#include "world/world.hpp"

#include "world/block/nature/air_block.hpp"
#include "world/block/nature/dirt_block.hpp"

namespace nith
{
    array<tuple<ivec3, BlockFace>, 6> Chunk::Directions = { {
        { { +1, 0, 0 }, BlockFace::EAST  },
        { { 0, +1, 0 }, BlockFace::UP    },
        { { 0, 0, +1 }, BlockFace::SOUTH },
        { { -1, 0, 0 }, BlockFace::WEST  },
        { { 0, -1, 0 }, BlockFace::DOWN  },
        { { 0, 0, -1 }, BlockFace::NORTH },
    } };

    array<array<tuple<vec3, vec2>, 2 * 3>, 6> Chunk::BlockVertices = { {
        // Right face
        {{
            { { +0.5f, -0.5f, +0.5f }, { 0, 0 } },  // top-left
            { { +0.5f, +0.5f, +0.5f }, { 0, 1 } },  // top-right      
            { { +0.5f, +0.5f, -0.5f }, { 1, 1 } },  // bottom-right          
            { { +0.5f, -0.5f, -0.5f }, { 1, 0 } },  // bottom-left
        }},
        // Top face
        {{
            { { -0.5f, +0.5f, +0.5f }, { 0, 0 } },  // bottom-right
            { { -0.5f, +0.5f, -0.5f }, { 0, 1 } },  // bottom-left  
            { { +0.5f, +0.5f, -0.5f }, { 1, 1 } },  // top-left    
            { { +0.5f, +0.5f, +0.5f }, { 1, 0 } },  // top-right

        }},
        // Front face
        {{
            { { -0.5f, -0.5f, +0.5f }, { 0, 0 } },  // top-right
            { { -0.5f, +0.5f, +0.5f }, { 0, 1 } },  // bottom-right        
            { { +0.5f, +0.5f, +0.5f }, { 1, 1 } },  // bottom-left
            { { +0.5f, -0.5f, +0.5f }, { 1, 0 } },  // top-left       
        }},
        // Left face
        {{
            { { -0.5f, -0.5f, -0.5f }, { 0, 0 } }, // bottom-left
            { { -0.5f, +0.5f, -0.5f }, { 0, 1 } }, // top-left       
            { { -0.5f, +0.5f, +0.5f }, { 1, 1 } }, // top-right
            { { -0.5f, -0.5f, +0.5f }, { 1, 0 } }, // bottom-right

        }},
        // Bottom face
        {{
            { { -0.5f, -0.5f, -0.5f }, { 0, 0 } }, // bottom-left
            { { -0.5f, -0.5f, +0.5f }, { 0, 1 } }, // top-left        
            { { +0.5f, -0.5f, +0.5f }, { 1, 1 } }, // top-right
            { { +0.5f, -0.5f, -0.5f }, { 1, 0 } }, // bottom-right
        }},
        // Back face
        {{
            { { +0.5f, -0.5f, -0.5f }, { 0, 0 } }, // bottom-left
            { { +0.5f, +0.5f, -0.5f }, { 0, 1 } }, // bottom-right    
            { { -0.5f, +0.5f, -0.5f }, { 1, 1 } }, // top-right              
            { { -0.5f, -0.5f, -0.5f }, { 1, 0 } }, // top-left      
        }},
    } };
    
    ChunkData::ChunkData() : world(nullptr)
    {
        packed_block air = Block::Pack(AirBlock{});
        for (u32 i = 0; i < CHUNK_VOLUME; ++i)
            blocks.set(i, air);
    }

    ChunkRenderer::ChunkRenderer() : dirty(true), mesh(true)
    {
        mesh.setAttributes({
            { Type::VEC3 }, // position
            { Type::VEC2 }, // uv
            { Type::UINT }, // tex_id
        });
    }

    ChunkPosition& Chunk::getPosition()
    {
        return this->getConstComponent<ChunkArchetype, ChunkPosition>();
    }

    packed_block Chunk::getPackedBlock(const uvec3& pos) const
    {
        auto& data = this->getConstComponent<ChunkArchetype, ChunkData>();
        return data.blocks.get(Chunk::BlockPosToIndex(pos));
    }

    IBlock Chunk::getBlock(const uvec3& pos) const
    {
        return Block::Unpack(getPackedBlock(pos));
    }

    void Chunk::setBlock(const uvec3& pos, const packed_block& block)
    {
        auto& data = this->getComponent<ChunkArchetype, ChunkData>();
        data.blocks.set(Chunk::BlockPosToIndex(pos), block);
    }

    void Chunk::tick(const f32& deltaTime)
    {
    }

    void Chunk::render(const f32& deltaTime)
    {
        auto& renderer = this->getConstComponent<ChunkArchetype, ChunkRenderer>();
        renderer.mesh.drawTriangles();
    }

    World& Chunk::getWorld()
    {
        return *this->getComponent<ChunkArchetype, ChunkData>().world;
    }

    void Chunk::generateMesh()
    {
        auto& renderer = this->getComponent<ChunkArchetype, ChunkRenderer>();
        auto& mesh = renderer.mesh;

        vector<BlockVertex> vertices;

        u32 drawCount = 0;
        vec3 origin = { CHUNK_SIZE / 2, CHUNK_SIZE / 2, CHUNK_SIZE / 2 };
        for (u32 x = 0; x < CHUNK_SIZE; ++x)
            for (u32 y = 0; y < CHUNK_SIZE; ++y)
                for (u32 z = 0; z < CHUNK_SIZE; ++z)
                {
                    ivec3 pos{ x, y, z };
                    IBlock block = getBlock(pos);
                    
                    if (block->getMaterial() == BlockMaterial::AIR) continue;

                    for (auto& [dir, face] : Chunk::Directions)
                    {
                        IBlock neighborBlock = getBlockOptimally(pos + dir);

                        //if (neighborBlock == nullptr) continue;

                        bool isVisible = block->canSeeThrough(face) || !neighborBlock
                            || neighborBlock->canSeeThrough(Block::GetOppositeFace(face));

                        if (!isVisible) continue;
  
                        u32 texId = block->getTextureId(face);
                        for (auto& [vertPos, vertUv] : BlockVertices[u32(face)])
                        {
                            vertices.push_back({
                                vertPos + vec3(pos) - origin,
                                vertUv, texId
                            });
                        }

                        drawCount += 6;
                    }
                }
        mesh.setVertices(vertices.data(), vertices.size());
        mesh.setDrawCount(drawCount);
    }

    IBlock Chunk::getBlockOptimally(const ivec3& pos)
    {
        bool isInside = pos.x >= 0 && pos.x < CHUNK_SIZE
            && pos.y >= 0 && pos.y < CHUNK_SIZE
            && pos.z >= 0 && pos.z < CHUNK_SIZE;
        ivec3 chunkPos = getPosition();
        return isInside ? getBlock(pos) : getWorld().getBlock(ivec3(chunkPos.x * CHUNK_SIZE, chunkPos.y * CHUNK_SIZE, chunkPos.z * CHUNK_SIZE) + pos);
    }

    u32 Chunk::BlockPosToIndex(const uvec3& pos)
    {
        return pos.x * CHUNK_SIZE * CHUNK_SIZE + pos.y * CHUNK_SIZE + pos.z;
    }

} // namespace nith
