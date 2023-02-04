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

    array<array<vec3, 2 * 3>, 6> Chunk::BlockVertices = { {
        // Right face
        {{
            { +0.5f, +0.5f, +0.5f },  // top-left
            { +0.5f, +0.5f, -0.5f },  // top-right      
            { +0.5f, -0.5f, -0.5f },  // bottom-right          
            { +0.5f, -0.5f, -0.5f },  // bottom-right
            { +0.5f, -0.5f, +0.5f },  // bottom-left
            { +0.5f, +0.5f, +0.5f }   // top-left
        }},
        // Top face
        {{
            { -0.5f, +0.5f, -0.5f },  // top-left
            { +0.5f, +0.5f, -0.5f },  // top-right
            { +0.5f, +0.5f, +0.5f },  // bottom-right                 
            { +0.5f, +0.5f, +0.5f },  // bottom-right
            { -0.5f, +0.5f, +0.5f },  // bottom-left  
            { -0.5f, +0.5f, -0.5f }   // top-left   
        }},
        // Front face
        {{
            { -0.5f, -0.5f, +0.5f },  // bottom-left
            { +0.5f, +0.5f, +0.5f },  // top-right
            { +0.5f, -0.5f, +0.5f },  // bottom-right        
            { +0.5f, +0.5f, +0.5f },  // top-right
            { -0.5f, -0.5f, +0.5f },  // bottom-left
            { -0.5f, +0.5f, +0.5f }   // top-left       
        }},
        // Left face
        {{
            { -0.5f, +0.5f, +0.5f },  // top-right
            { -0.5f, -0.5f, -0.5f },  // bottom-left
            { -0.5f, +0.5f, -0.5f },  // top-left       
            { -0.5f, -0.5f, -0.5f },  // bottom-left
            { -0.5f, +0.5f, +0.5f },  // top-right
            { -0.5f, -0.5f, +0.5f }   // bottom-right
        }},
        // Bottom face
        {{
            { -0.5f, -0.5f, -0.5f },  // top-right
            { +0.5f, -0.5f, +0.5f },  // bottom-left
            { +0.5f, -0.5f, -0.5f },  // top-left        
            { +0.5f, -0.5f, +0.5f },  // bottom-left
            { -0.5f, -0.5f, -0.5f },  // top-right
            { -0.5f, -0.5f, +0.5f }   // bottom-right
        }},
        // Back face
        {{
            { -0.5f, -0.5f, -0.5f },  // bottom-left
            { +0.5f, -0.5f, -0.5f },  // bottom-right    
            { +0.5f, +0.5f, -0.5f },  // top-right              
            { +0.5f, +0.5f, -0.5f },  // top-right
            { -0.5f, +0.5f, -0.5f },  // top-left
            { -0.5f, -0.5f, -0.5f }   // bottom-left          
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
            { Type::VEC3 }  // color
        });
    }

    ivec3 Chunk::getPosition() const
    {
        return this->getConstComponent<ChunkArchetype, ChunkPosition>();
    }

    ivec3 Chunk::getWorldPosition() const
    {
        ivec3 pos = getPosition();
        return { pos.x * CHUNK_SIZE, pos.y * CHUNK_SIZE, pos.z * CHUNK_SIZE };
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

    void Chunk::render() const
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

        vector<float> vertices;

        u32 drawCount = 0;
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
                        
                        uvec3 vertColor = { x * 4 + 64, y * 4 + 64, z * 4 + 64 };

                        for (auto& vertPos : BlockVertices[u32(face)])
                        {
                            vertices.insert(vertices.end(), {
                                vertPos.x + x,
                                vertPos.y + y,
                                vertPos.z + z,
                                vertColor.x / 255.0f,
                                vertColor.y / 255.0f,
                                vertColor.z / 255.0f
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
        return isInside ? getBlock(pos) : getWorld().getBlock(getWorldPosition() + pos);
    }

    u32 Chunk::BlockPosToIndex(const uvec3& pos)
    {
        return pos.x * CHUNK_SIZE * CHUNK_SIZE + pos.y * CHUNK_SIZE + pos.z;
    }

} // namespace nith
