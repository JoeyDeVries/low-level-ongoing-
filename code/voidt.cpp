/*******************************************************************
** Copyright (C) 2015-2016 {Joey de Vries} {joey.d.vries@gmail.com}
**
** This code is part of Voidt.
** https://github.com/JoeyDeVries/Voidt
**
** Voidt is free software: you can redistribute it and/or modify it
** under the terms of the CC BY-NC 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
*******************************************************************/
#include "voidt.h"

#include "intrinsics.h"
#include "map.cpp"

internal void DrawRectangle(game_offscreen_buffer *buffer, vector2D min, vector2D max, real32 r, real32 g, real32 b)
{  
    int32 minX = RoundReal32ToInt32(min.X);
    int32 minY = RoundReal32ToInt32(min.Y);
    int32 maxX = RoundReal32ToInt32(max.X);
    int32 maxY = RoundReal32ToInt32(max.Y);
    
    if(minX < 0) minX = 0;
    if(minY < 0) minY = 0;
    if(maxX > buffer->Width) maxX = buffer->Width;
    if(maxY > buffer->Height) maxY = buffer->Height;
    
    // BIT PATTERN: 0x AA RR GG BB
    uint32 color = (RoundReal32ToUint32(r * 255.0f) << 16) |
                   (RoundReal32ToUint32(g * 255.0f) << 8) |
                   (RoundReal32ToUint32(b * 255.0f) << 0);
        
    uint8* row = (uint8*)buffer->Memory + minX * buffer->BytesPerPixel + minY * buffer->Pitch;
    
    for(int y = minY; y < maxY; ++y)
    {
        uint32* pixel = (uint32*)row;
        for(int x = minX; x < maxX; ++x)
        {            
            *pixel++ = color;        
        }          
        row += buffer->Pitch;
    }
    
}

internal void DrawBitmap(game_offscreen_buffer *buffer, loaded_bitmap *bitmap, real32 X, real32 Y, int32 alignX = 0, int32 alignY = 0, real32 alpha = 1.0f)
{
    X -= (real32)alignX;
    Y -= (real32)alignY;
    
    int32 minX = RoundReal32ToInt32(X);
    int32 minY = RoundReal32ToInt32(Y);
    int32 maxX = RoundReal32ToInt32(X + bitmap->Width);
    int32 maxY = RoundReal32ToInt32(Y + bitmap->Height);
    
    int32 sourceOffsetX = 0; // fixing clipping
    int32 sourceOffsetY = 0;
    if(minX < 0) 
    {
        sourceOffsetX = -minX;
        minX = 0;
    }
    if(minY < 0)
    {
        sourceOffsetY = -minY;
        minY = 0;
    }
    if(maxX > buffer->Width)
    { 
        maxX = buffer->Width;
    }
    if(maxY > buffer->Height) 
    {
        maxY = buffer->Height;
    }
    
    
    uint32 *sourceRow = bitmap->Pixels + bitmap->Width*(bitmap->Height - 1); // start from top
    sourceRow += -sourceOffsetY*bitmap->Width + sourceOffsetX; // offset source access by however much we clipped 
    
    uint8 *destRow = (uint8*)buffer->Memory + minX * buffer->BytesPerPixel + minY * buffer->Pitch;
    
    for(int y = minY; y < maxY; ++y)
    {
        uint32 *dest = (uint32*)destRow;
        uint32 *source = (uint32*)sourceRow;
        for(int x = minX; x < maxX; ++x)
        {          
            real32 A = (real32)((*source >> 24) & 0xFF) / 255.0f;
            A *= alpha;
            real32 SR = (real32)((*source >> 16) & 0xFF);
            real32 SG = (real32)((*source >> 8) & 0xFF);
            real32 SB = (real32)((*source >> 0) & 0xFF);
            
            real32 DR = (real32)((*dest >> 16) & 0xFF);
            real32 DG = (real32)((*dest >> 8) & 0xFF);
            real32 DB = (real32)((*dest >> 0) & 0xFF);
    
            real32 R = (1.0f - A)*DR + A*SR;
            real32 G = (1.0f - A)*DG + A*SG;
            real32 B = (1.0f - A)*DB + A*SB;
            
            *dest = ((uint32)(R + 0.5) << 16) |
                    ((uint32)(G + 0.5) << 8) |
                    ((uint32)(B + 0.5) << 0);
    
            dest++; source++;
        }          
        destRow += buffer->Pitch;
        sourceRow -= bitmap->Width;
    }
    
    
}

void GameRender(game_offscreen_buffer *buffer, game_state *state)
{
    // int XOffset = 0;
    // int YOffset = 0;
    // render
    // uint8 *row = (uint8 *)screenBuffer->Memory; 
    // for(int Y = 0; Y < screenBuffer->Height; ++Y)
    // {
        // uint32 *Pixel = (uint32 *)row;
        // for(int X = 0; X < screenBuffer->Width; ++X)
        // {    
            // uint8 Blue  = (uint8)(X + xOffset);           // B
            // uint8 Green = (uint8)(Y + yOffset);           // G
            // uint8 Red   = (uint8)(X + yOffset + xOffset); // R
                        
            // *Pixel++ = (Red << 16) | (Green << 8) | Blue;
        // }
        // row += screenBuffer->Pitch;
    // }           

   
    
   
}



void GameOutputSound(game_sound_output_buffer *soundBuffer, game_state *gameState, int toneHz)
{
    // uint16 toneVolume = 3000;
    // uint16 wavePeriod = (uint16)(soundBuffer->SamplesPerSecond / toneHz);
    
    // int16 *sampleOut = soundBuffer->Samples;
    // for(int sampleIndex = 0; sampleIndex < soundBuffer->SampleCount; ++sampleIndex)
    // {
        // real32 sineValue = sinf((real32)gameState->tSine);
        // int16 sampleValue = (int16)(sineValue * toneVolume);
        // sampleValue = 0; // disable
        // *sampleOut++ = sampleValue;
        // *sampleOut++ = sampleValue;
        
        // gameState->tSine += 2.0f * Pi32 * 1.0f / wavePeriod;
    // }        
} 

// internal bool32 IsTileEmtpy(game_world *world, tile_map *tileMap, int32 testTileX, int32 testTileY)
// {
    // bool32 empty = false;
    
    // if(tileMap)
    // {
        // if(testTileX >= 0 && testTileX < world->TileCountX && testTileY >=0 && testTileY < world->TileCountY)
        // {
            // empty = GetTileValueUnchecked(world, tileMap, testTileX, testTileY) == 0;       
        // }
    // }
    // return empty;    
    
    
// }




#pragma pack(push, 1)
struct bitmap_header
{
    uint16 FileType;
    uint32 FileSize;
    uint16 Reserved1;
    uint16 Reserved2;
    uint32 BitmapOffset;
    uint32 Size;
    int32 Width;
    int32 Height;
    uint16 Planes;
    uint16 BitsPerPixel;
    uint32 Compression;
    uint32 SizeOfBitmap;
    int32 HorzResolution;
    int32 VertResolution;
    uint32 ColorsUsed;
    uint32 ColorsImportant;
    
    uint32 RedMask;
    uint32 GreenMask;
    uint32 BlueMask;
    
};
#pragma pack(pop)

internal loaded_bitmap DEBUGLoadBMP(thread_context *thread, debug_platform_read_entire_file *readEntireFile, char *fileName)
{
    loaded_bitmap result = {};
    
    debug_read_file_result readResult = readEntireFile(thread, fileName);    
    if(readResult.ContentSize != 0)
    {
        bitmap_header *header = (bitmap_header *)readResult.Contents;
        uint32 *pixels = (uint32*)((uint8*)readResult.Contents + header->BitmapOffset);
        result.Pixels = pixels;
        result.Width= header->Width;
        result.Height = header->Height;
        
        Assert(header->Compression == 3);
        
        // NOTE(Joey): byte order in memory is AA BB GG RR (AA first in lowest memory address), bottom upper_bound
        // CPU reads it in as: RR GG BB AA (first reads AA, then BB)
        // we need AA first so switch AA to the back for each pixel
        // !!!actually: byte order is determined by the header itself with 3 masks for each individual color
        
        uint32 redMask = header->RedMask;
        uint32 greenMask = header->GreenMask;
        uint32 blueMask = header->BlueMask;
        uint32 alphaMask = ~(redMask | greenMask | blueMask);
        
        uint32 redShift = 0;
        uint32 greenShift = 0;
        uint32 blueShift = 0;
        uint32 alphaShift = 0;              
        
        bool32 found = FindLeastSignificantSetBit(&redShift, redMask);
        Assert(found);
        found = FindLeastSignificantSetBit(&greenShift, greenMask);
        Assert(found);
        found = FindLeastSignificantSetBit(&blueShift, blueMask);
        Assert(found);
        found = FindLeastSignificantSetBit(&alphaShift, alphaMask);
        Assert(found);
        
        redShift = 16 - (int32)redShift;
        greenShift = 8 - (int32)greenShift;
        blueShift = 0 - (int32)blueShift;
        alphaShift = 24 - (int32)alphaShift;
        
        uint32 *sourceDest = pixels;
        for(int32 y = 0; y < result.Height; ++y)
        {
            for(int x = 0; x < result.Width; ++x)
            {
                uint32 C = *sourceDest;
#if 0                
                *sourceDest++ = (((C >> alphaShift) & 0xFF) << 24) | 
                                (((C >> redShift  ) & 0xFF) << 16) | 
                                (((C >> greenShift) & 0xFF) << 8)  | 
                                (((C >> blueShift ) & 0xFF) << 0); 
#else
                *sourceDest++ = (RotateLeft(C & redMask, redShift) |
                                 RotateLeft(C & greenMask, greenShift) |
                                 RotateLeft(C & blueMask, blueShift) |
                                 RotateLeft(C & alphaMask, alphaShift));
#endif
            }            
        }
    }
    return result;
}





internal void ChangeEntityResidence(game_state *gameState, uint32 entityIndex, entity_residence residence)
{
    if(residence == ENTITY_RESIDENCE_HIGH)
    {
        if(gameState->EntityResidence[entityIndex] != ENTITY_RESIDENCE_HIGH)
        {
            high_entity *highEntity = &gameState->HighEntities[entityIndex];
            dormant_entity *dormantEntity = &gameState->DormantEntities[entityIndex];
            
            tile_map_difference diff = Subtract(gameState->World->TileMap, &dormantEntity->Position, &gameState->CameraPos);
            highEntity->Position = { diff.dX, diff.dY };
            highEntity->Velocity = { 0, 0 };
            highEntity->AbsTileZ = dormantEntity->Position.AbsTileZ;
            highEntity->FacingDirection = 0;
        }
    }
    
    gameState->EntityResidence[entityIndex] = residence;
}

inline game_entity GetEntity(game_state *gameState, entity_residence residence, uint32 index)
{
    game_entity result = {};
    if(index > 0 && index < gameState->EntityCount)
    {
        if(gameState->EntityResidence[index] < residence)
        {
            ChangeEntityResidence(gameState, index, residence);
            Assert(gameState->EntityResidence[index] >= residence);
        }
        
        result.Residence = residence;
        result.Dormant   = &gameState->DormantEntities[index];
        result.Low       = &gameState->LowEntities[index];
        result.High      = &gameState->HighEntities[index];
    }
    return result;
}

internal uint32 AddEntity(game_state *gameState)
{
    uint32 index = gameState->EntityCount++;
    
    Assert(gameState->EntityCount < ArrayCount(gameState->DormantEntities));
    Assert(gameState->EntityCount < ArrayCount(gameState->LowEntities));
    Assert(gameState->EntityCount < ArrayCount(gameState->HighEntities));
    
    gameState->EntityResidence[index] = ENTITY_RESIDENCE_DORMANT;
    gameState->DormantEntities[index] = {};
    gameState->LowEntities[index] = {};
    gameState->HighEntities[index] = {};
    
    return index;
}


internal void InitializePlayer(game_state *gameState, uint32 entityIndex)
{
    game_entity player = GetEntity(gameState, ENTITY_RESIDENCE_DORMANT, entityIndex);
    
    player.Dormant->Position.AbsTileX = 1;
    player.Dormant->Position.AbsTileY = 3;
    // player.Dormant.Position.AbsTileZ = 0;
    player.Dormant->Position.Offset.X = 0.0f;
    player.Dormant->Position.Offset.X = 0.0f;
    player.Dormant->Collides = true;
    
    player.Dormant->Height = 0.5f;
    player.Dormant->Width = 1.0f;
    
    ChangeEntityResidence(gameState, entityIndex, ENTITY_RESIDENCE_HIGH);
    
    if(GetEntity(gameState, ENTITY_RESIDENCE_DORMANT, gameState->CameraFollowingEntityIndex).Residence == ENTITY_RESIDENCE_NONEXISTANT)
    {
        gameState->CameraFollowingEntityIndex = entityIndex;
    }    
}

internal bool32 TestWall(real32 wallX, real32 relX, real32 relY, real32 playerDeltaX, real32 playerDeltaY, real32 *tMin, real32 minY, real32 maxY)
{
    real32 tEpsilon = 0.01f;
    if(playerDeltaX != 0.0f)
    {
        real32 tResult = (wallX - relX) / playerDeltaX;
        real32 y = relY + tResult*playerDeltaY;
        if(tResult >= 0.0f && *tMin > tResult)
        {
            if(y >= minY && y <= maxY)
            {
                *tMin = Maximum(0.0f, tResult - tEpsilon);
                return true;
            }
        }        
    }    
    return false;
}    



internal void MovePlayer(game_state *gameState, game_entity player, real32 dt, vector2D acceleration)
{
    tile_map *tileMap = gameState->World->TileMap;
    
    real32 accelerationLength = LengthSq(acceleration); // get squared length, still valid to check length > 1.0 and much cheaper (no sqrt)
    if(accelerationLength > 1.0f)
    {
        acceleration *= 1.0f / SquareRoot(accelerationLength);
    }
    
    real32 speed = 50.0f;
    acceleration *= speed;
            
    // add friction (approximation) to acceleration
    acceleration += -8.0f*player.High->Velocity;
            
    // update player position (use velocity at begin of frame)
    vector2D oldPlayerPos = player.High->Position;
    vector2D playerDelta = 0.5f * acceleration*Square(dt) + player.High->Velocity*dt;
    vector2D newPlayerPos = oldPlayerPos + playerDelta;
    player.High->Velocity += acceleration * dt;
            
         
    // detect collisions
    

    /*
    uint32 minTileX = Minimum(oldPlayerPos.AbsTileX, newPlayerPos.AbsTileX);
    uint32 minTileY = Minimum(oldPlayerPos.AbsTileY, newPlayerPos.AbsTileY);
    uint32 maxTileX = Maximum(oldPlayerPos.AbsTileX, newPlayerPos.AbsTileX);
    uint32 maxTileY = Maximum(oldPlayerPos.AbsTileY, newPlayerPos.AbsTileY);

    // get width/height of entity in tiles (also base tiles to check on minkowski sum)
    uint32 entityTileWidth = CeilReal32ToInt32(player.Dormant->Width / tileMap->TileSideInMeters);
    uint32 entityTileHeight = CeilReal32ToInt32(player.Dormant->Height / tileMap->TileSideInMeters);
    
    minTileX -= entityTileWidth;
    minTileY -= entityTileHeight;
    maxTileX += entityTileWidth;
    maxTileY += entityTileHeight;
    Assert(maxTileX - minTileX < 32);
    Assert(maxTileY - minTileY < 32);
    
    uint32 absTileZ = player.Dormant->Position.AbsTileZ;
    */

       

    real32 tRemaining = 1.0f;      
    for(uint32 i = 0; i < 4 && tRemaining > 0.0f; ++i)
    {
        real32 tMin = 1.0f;
        vector2D wallNormal = {};                
        uint32 hitEntityIndex = 0;
        
        for(uint32 entityIndex = 1; entityIndex < gameState->EntityCount; ++entityIndex)
        {
            /* NOTE(Joey):
               
              We're going to use the Minkowski sum for collision detection.
              The idea is to reduce the relative complex shapes of objects to check
              by adding the shape of one along the exterior of the other to reduce
              one of the shapes to a single point. Collision detection then simply
              becomes a point-shape algorithm, which is relatively easier to implement.
             
              At the moment we simply only deal with quads (for more complex shapes we
              want to use the GJK algorithm). We solve this by adding entity-quad's 
              halfwidths to the shape(s) to test, reducing the player entity to a 
              point.
            */               
            
            game_entity testEntity = GetEntity(gameState, ENTITY_RESIDENCE_HIGH, entityIndex);
            if(testEntity.High != player.High)
            {                
                if(testEntity.Dormant->Collides)
                {
                    real32 diameterW = testEntity.Dormant->Width + player.Dormant->Width;
                    real32 diameterH = testEntity.Dormant->Height + player.Dormant->Height;
                    vector2D minCorner = -0.5f*vector2D{diameterW, diameterH};
                    vector2D maxCorner = 0.5f*vector2D{diameterW, diameterH};

                    vector2D rel = player.High->Position - testEntity.High->Position;
             
                    if(TestWall(minCorner.X, rel.X, rel.Y, playerDelta.X, playerDelta.Y,
                                &tMin, minCorner.Y, maxCorner.Y))
                    {
                        wallNormal = {-1, 0};
                        hitEntityIndex = entityIndex;
                    }
                
                    if(TestWall(maxCorner.X, rel.X, rel.Y, playerDelta.X, playerDelta.Y,
                                &tMin, minCorner.Y, maxCorner.Y))
                    {
                        wallNormal = {1, 0};
                        hitEntityIndex = entityIndex;
                    }
                
                    if(TestWall(minCorner.Y, rel.Y, rel.X, playerDelta.Y, playerDelta.X,
                                &tMin, minCorner.X, maxCorner.X))
                    {
                        wallNormal = {0, -1};
                        hitEntityIndex = entityIndex;
                    }
                
                    if(TestWall(maxCorner.Y, rel.Y, rel.X, playerDelta.Y, playerDelta.X,
                                &tMin, minCorner.X, maxCorner.X))
                    {
                        wallNormal = {0, 1};
                        hitEntityIndex = entityIndex;
                    }     
                }
            }            
        }    
        player.High->Position += tMin*playerDelta;    
        // special collision events, should something happen? (like stairs, cinematic start areas)
        if(hitEntityIndex)
        {
            player.High->Velocity = player.High->Velocity - 1*InnerProduct(player.High->Velocity, wallNormal)*wallNormal;
            playerDelta = playerDelta - 1*InnerProduct(playerDelta, wallNormal)*wallNormal;
            tRemaining -= tMin*tRemaining;
            
            game_entity hitEntity = GetEntity(gameState, ENTITY_RESIDENCE_DORMANT, hitEntityIndex);
            hitEntity.High->AbsTileZ += hitEntity.Dormant->dAbsTileZ;
        }
        else
            break;
    }                         
    
    // determine new facing direction
    if(player.High->Velocity.X == 0.0f && player.High->Velocity.Y == 0.0f)
    {
        // NOTE(Joey): leave facing direction to earlier set facing direction
    }
    else if(Absolute(player.High->Velocity.X) > Absolute(player.High->Velocity.Y))
    {
        player.High->FacingDirection = player.High->Velocity.X > 0 ? 0 : 2;            
    }
    else
    {
        player.High->FacingDirection = player.High->Velocity.Y > 0 ? 1 : 3;
    }       
    player.Dormant->Position = MapIntoTileSpace(gameState->World->TileMap, gameState->CameraPos, player.High->Position);
}

extern "C" GAME_UPDATE_AND_RENDER(GameUpdateAndRender)
{
    // Assert((&input->Controllers[0].Back - &input->Controllers[0].Buttons[0]) == ArrayCount(input->Controllers[0].Buttons) - 1); // check if button array matches union struct members
    Assert(sizeof(game_state) <= memory->PermanentStorageSize);      
    
    game_state *gameState = (game_state*)memory->PermanentStorage;    
    if(!memory->IsInitialized)
    {              
        // NOTE(Joey): Reserve entity slot 0 for the null entity
        AddEntity(gameState);

        gameState->BackDrop = DEBUGLoadBMP(thread, memory->DEBUGPlatformReadEntireFile, "test/test_background.bmp");
        gameState->Shadow = DEBUGLoadBMP(thread, memory->DEBUGPlatformReadEntireFile, "test/test_hero_shadow.bmp");
        
        gameState->HeroBitmaps[0].Head = DEBUGLoadBMP(thread, memory->DEBUGPlatformReadEntireFile, "test/test_hero_right_head.bmp");
        gameState->HeroBitmaps[0].Torso = DEBUGLoadBMP(thread, memory->DEBUGPlatformReadEntireFile, "test/test_hero_right_torso.bmp");
        gameState->HeroBitmaps[0].Cape = DEBUGLoadBMP(thread, memory->DEBUGPlatformReadEntireFile, "test/test_hero_right_cape.bmp");
        gameState->HeroBitmaps[0].AlignX = 72;
        gameState->HeroBitmaps[0].AlignY = 182;
        
        gameState->HeroBitmaps[1].Head = DEBUGLoadBMP(thread, memory->DEBUGPlatformReadEntireFile, "test/test_hero_back_head.bmp");
        gameState->HeroBitmaps[1].Torso = DEBUGLoadBMP(thread, memory->DEBUGPlatformReadEntireFile, "test/test_hero_back_torso.bmp");
        gameState->HeroBitmaps[1].Cape = DEBUGLoadBMP(thread, memory->DEBUGPlatformReadEntireFile, "test/test_hero_back_cape.bmp");
        gameState->HeroBitmaps[1].AlignX = 72;
        gameState->HeroBitmaps[1].AlignY = 182;
        
        gameState->HeroBitmaps[2].Head = DEBUGLoadBMP(thread, memory->DEBUGPlatformReadEntireFile, "test/test_hero_left_head.bmp");
        gameState->HeroBitmaps[2].Torso = DEBUGLoadBMP(thread, memory->DEBUGPlatformReadEntireFile, "test/test_hero_left_torso.bmp");
        gameState->HeroBitmaps[2].Cape = DEBUGLoadBMP(thread, memory->DEBUGPlatformReadEntireFile, "test/test_hero_left_cape.bmp");
        gameState->HeroBitmaps[2].AlignX = 72;
        gameState->HeroBitmaps[2].AlignY = 182;
        
        gameState->HeroBitmaps[3].Head = DEBUGLoadBMP(thread, memory->DEBUGPlatformReadEntireFile, "test/test_hero_front_head.bmp");
        gameState->HeroBitmaps[3].Torso = DEBUGLoadBMP(thread, memory->DEBUGPlatformReadEntireFile, "test/test_hero_front_torso.bmp");
        gameState->HeroBitmaps[3].Cape = DEBUGLoadBMP(thread, memory->DEBUGPlatformReadEntireFile, "test/test_hero_front_cape.bmp");
        gameState->HeroBitmaps[3].AlignX = 72;
        gameState->HeroBitmaps[3].AlignY = 182;

        gameState->CameraPos.AbsTileX = 17/2;
        gameState->CameraPos.AbsTileY = 9/2;
        
        // generate world tile map          
        InitializeArena(&gameState->WorldArena, memory->PermanentStorageSize - sizeof(game_state), (uint8*)memory->PermanentStorage + sizeof(game_state));
        
        gameState->World = PushStruct(&gameState->WorldArena, game_world);
        game_world *world = gameState->World;
        world->TileMap = PushStruct(&gameState->WorldArena, tile_map);
                
        tile_map *tileMap = world->TileMap;
        
        tileMap->ChunkShift = 4;
        tileMap->ChunkMask = (1 << tileMap->ChunkShift) - 1;
        tileMap->ChunkDim = (1 << tileMap->ChunkShift);
        
        tileMap->TileChunkCountX = 128;
        tileMap->TileChunkCountY = 128;
        tileMap->TileChunkCountZ = 2;
        tileMap->TileChunks = PushArray(&gameState->WorldArena, tileMap->TileChunkCountX*tileMap->TileChunkCountY*tileMap->TileChunkCountZ, tile_chunk);       
        
        tileMap->TileSideInMeters = 1.4f;

        uint32 tilesPerWidth = 17;
        uint32 tilesPerHeight = 9;
        
        uint32 screenX = 0;        
        uint32 screenY = 0;
        
        uint32 absTileZ = 0;
        bool doorTop = false;
        bool doorBottom = false;
        bool doorLeft = false;
        bool doorRight = false;
        bool doorUp  = false;
        bool doorDown = false;
        for(uint32 screenIndex = 0; screenIndex < 100; ++screenIndex)
        {
            uint32 randomChoice = 0;
            if(doorUp || doorDown)
                randomChoice = (885447 * screenIndex + (screenIndex + 7) * 7637 / 937) % 2;
            else
                randomChoice = (885447 * screenIndex + (screenIndex + 7) * 7637 / 937) % 3;

            bool createdZDoor = false;
            if(randomChoice == 2)
            {
                createdZDoor = true;
                if (absTileZ == 0)
                    doorUp = true;
                else
                    doorDown = true;
            }                
            else if (randomChoice == 1)
                doorRight = true;
            else
                doorTop = true;
            
            
            for(uint32 tileY = 0; tileY < tilesPerHeight; ++tileY)
            {
                for(uint32 tileX = 0; tileX< tilesPerWidth; ++tileX)
                {
                    uint32 absTileX = screenX*tilesPerWidth + tileX;
                    uint32 absTileY = screenY*tilesPerHeight + tileY;
                    
                    uint32 tileValue = 1;
                    if((tileX == 0) && (!doorLeft || (tileY != (tilesPerHeight / 2))))
                    {
                        tileValue = 2;
                    }
                    if((tileX == (tilesPerWidth - 1)) && (!doorRight || (tileY != (tilesPerHeight / 2))))
                    {
                        tileValue = 2;
                    }
                    if((tileY == 0) && (!doorBottom || (tileX != (tilesPerWidth / 2))))
                    {
                        tileValue = 2;
                    }
                    if((tileY == (tilesPerHeight - 1)) && (!doorTop || (tileX != (tilesPerWidth / 2))))
                    {
                        tileValue = 2;
                    }
                    
                    if((tileX == 10) && (tileY == 6))
                    {
                        if(doorUp) 
                        {
                            tileValue = 3;
                        }
                        if(doorDown)
                        {
                            tileValue = 4;
                        }
                    }                      
                    
                    SetTileValue(&gameState->WorldArena, world->TileMap, absTileX, absTileY, absTileZ, tileValue);
                }
            }
            
                      
            doorLeft = doorRight;
            doorBottom = doorTop;
            
            if(createdZDoor)
            {
                doorDown = !doorDown;
                doorUp = !doorUp;
            }
            else
            {
                doorUp = false;
                doorDown = false;
            }
  
            
            doorRight = false;
            doorTop = false;
            
            if(randomChoice == 2)
            {
                if(absTileZ == 0) 
                {
                    absTileZ = 1;
                }
                else 
                {
                    absTileZ = 0;
                }
            }
            else if(randomChoice == 1)
            {
                screenX += 1;
            }
            else
            {
                screenY += 1;
            }
        }

        memory->IsInitialized = true;
    }            
    
  
    
    // handle game input 
    for(int controllerIndex = 0; controllerIndex < ArrayCount(input->Controllers); ++controllerIndex)
    {
        game_controller_input *controller = GetController(input, controllerIndex);
        game_entity controllingEntity = GetEntity(gameState, ENTITY_RESIDENCE_HIGH, gameState->PlayerControllerIndex[controllerIndex]);
        if(controllingEntity.Residence != ENTITY_RESIDENCE_NONEXISTANT)
        {
            vector2D dAcceleration = {};
            if(controller->IsAnalog)
            {
                // analog movement tuning
                dAcceleration = { controller->StickAverageX, controller->StickAverageY };
            }
            else
            {
                // digital movement tuning
                if(controller->MoveUp.EndedDown)
                {
                    dAcceleration.Y =  1.0f;
                }
                if(controller->MoveDown.EndedDown)
                {
                    dAcceleration.Y = -1.0f;
                }
                if(controller->MoveLeft.EndedDown)
                {
                    dAcceleration.X = -1.0f;
                }
                if(controller->MoveRight.EndedDown)
                {
                    dAcceleration.X =  1.0f;
                }                                
            }
            
            if(controller->ActionUp.EndedDown)
                controllingEntity.High->dZ = 3.0f;
            
            MovePlayer(gameState, controllingEntity, input->dtPerFrame, dAcceleration);
        }
        else
        {
            if(controller->Start.EndedDown)
            {
                uint32 entityIndex = AddEntity(gameState);
                InitializePlayer(gameState, entityIndex);
                gameState->PlayerControllerIndex[controllerIndex] = entityIndex;                
            }
        }
    }
    
    // move camera based on whichever entity we follow
    tile_map *tileMap = gameState->World->TileMap;
    
    vector2D entityOffsetPerFrame = {};
    game_entity cameraFollowingEntity = GetEntity(gameState, ENTITY_RESIDENCE_HIGH, gameState->CameraFollowingEntityIndex);
    if(cameraFollowingEntity.Residence != ENTITY_RESIDENCE_NONEXISTANT)
    {
        tile_map_position oldCameraP = gameState->CameraPos;
        
        gameState->CameraPos.AbsTileZ = cameraFollowingEntity.Dormant->Position.AbsTileZ;
    
        if(cameraFollowingEntity.High->Position.X > 9.0f*tileMap->TileSideInMeters)
            gameState->CameraPos.AbsTileX += 17;
        if(cameraFollowingEntity.High->Position.X < -9.0f*tileMap->TileSideInMeters)
            gameState->CameraPos.AbsTileX -= 17;
        if(cameraFollowingEntity.High->Position.Y > 5.0f*tileMap->TileSideInMeters)
            gameState->CameraPos.AbsTileY += 9;
        if(cameraFollowingEntity.High->Position.Y < -5.0f*tileMap->TileSideInMeters)
            gameState->CameraPos.AbsTileY -= 9;
        
        // determine how far camera moved, and update all entities back to proper camera space
        tile_map_difference dCameraPos = Subtract(tileMap, &gameState->CameraPos, &oldCameraP);
        entityOffsetPerFrame = { -dCameraPos.dX, -dCameraPos.dY };
    }   
    
                        
      
        
    // render to screen memory      
    int32  TileSideInPixels= 60;
    real32 MetersToPixels = TileSideInPixels / tileMap->TileSideInMeters;       
        
    real32 LowerLeftX = -TileSideInPixels / 2.0f;
    real32 LowerLeftY = screenBuffer->Height;
    
    vector2D screenCenter = { 0.5f*(real32)screenBuffer->Width, 0.5f*(real32)screenBuffer->Height };

    // DrawRectangle(screenBuffer, 0.0f, 0.0f, (real32)screenBuffer->Width, (real32)screenBuffer->Height, 1.0f, 0.5f, 0.0f);
    DrawBitmap(screenBuffer, &gameState->BackDrop, 0.0f, 0.0f);
    
    for(int32 relRow = -10; relRow < 10; ++relRow)
    {
        for(int32 relCol = -20; relCol < 20; ++relCol)
        {
            uint32 col = relCol + gameState->CameraPos.AbsTileX;
            uint32 row = relRow + gameState->CameraPos.AbsTileY;
            uint32 tileID = GetTileValue(tileMap, col, row, gameState->CameraPos.AbsTileZ);
            
            if(tileID > 1)
            {
                real32 gray = tileID == 2 ? 1.0f : 0.5f;
                
                if(tileID > 2)
                {
                    gray = 0.1f;
                }
                
                if(col == gameState->CameraPos.AbsTileX && row == gameState->CameraPos.AbsTileY)
                {
                    gray = 0.25f;
                }
             
                vector2D center = { 
                    screenCenter.X - MetersToPixels*gameState->CameraPos.Offset.X + ((real32)relCol * TileSideInPixels),
                    screenCenter.Y + MetersToPixels*gameState->CameraPos.Offset.Y - ((real32)relRow * TileSideInPixels)
                };
                vector2D halfWidths = { 0.5f*TileSideInPixels, 0.5f*TileSideInPixels };
                vector2D min = center - halfWidths;
                vector2D max = center + halfWidths;
                DrawRectangle(screenBuffer, min, max, 0.75f, gray, 0.75f);
            }
            
        }        
    }
    
    // entity *player = gameState->Entities;
    for(uint32 entityIndex = 0; entityIndex < gameState->EntityCount; ++entityIndex)
    {
        if(gameState->EntityResidence[entityIndex] == ENTITY_RESIDENCE_HIGH)
        {
            high_entity *highEntity = &gameState->HighEntities[entityIndex];            
            low_entity *lowEntity = &gameState->LowEntities[entityIndex];            
            dormant_entity *dormantEntity = &gameState->DormantEntities[entityIndex];           

            highEntity->Position += entityOffsetPerFrame;            
            
            // jump
            real32 dt = input->dtPerFrame;
            real32 ddZ = -9.81f;
            highEntity->Z +=  0.5f*ddZ*Square(dt) + highEntity->dZ*dt;
            highEntity->dZ = highEntity->dZ + ddZ*dt;
            if(highEntity->Z < 0)
                highEntity->Z = 0.0f;
            real32 alpha = 1.0f - highEntity->Z;
            if(alpha < 0)
                alpha = 0.0f;
            
            real32 playerGroundPointX = screenCenter.X + MetersToPixels*highEntity->Position.X;
            real32 playerGroundPointY = screenCenter.Y - MetersToPixels*highEntity->Position.Y;
            real32 playerLeft = playerGroundPointX - 0.5f*MetersToPixels*dormantEntity->Width;
            real32 playerTop = playerGroundPointY - 0.5f*MetersToPixels*dormantEntity->Height;
            real32 playerR = 1.0f;
            real32 playerG = 0.5f;
            real32 playerB = 0.0f;       
            real32 Z = -MetersToPixels*highEntity->Z;
            
            DrawRectangle(screenBuffer, { playerLeft, playerTop }, { playerLeft + MetersToPixels*dormantEntity->Width, playerTop + MetersToPixels*dormantEntity->Height}, playerR, playerG, playerB);
            uint32 facingDirection = highEntity->FacingDirection;
            DrawBitmap(screenBuffer, &gameState->Shadow, playerGroundPointX, playerGroundPointY, gameState->HeroBitmaps[facingDirection].AlignX, gameState->HeroBitmaps[facingDirection].AlignY, alpha);            

            DrawBitmap(screenBuffer, &gameState->HeroBitmaps[facingDirection].Torso, playerGroundPointX, playerGroundPointY + Z, gameState->HeroBitmaps[facingDirection].AlignX, gameState->HeroBitmaps[facingDirection].AlignY);
            DrawBitmap(screenBuffer, &gameState->HeroBitmaps[facingDirection].Cape, playerGroundPointX, playerGroundPointY + Z, gameState->HeroBitmaps[facingDirection].AlignX, gameState->HeroBitmaps[facingDirection].AlignY);
            DrawBitmap(screenBuffer, &gameState->HeroBitmaps[facingDirection].Head, playerGroundPointX, playerGroundPointY + Z, gameState->HeroBitmaps[facingDirection].AlignX, gameState->HeroBitmaps[facingDirection].AlignY);            
        }        
    }
    

    // GameRender(screenBuffer, gameState);
}

extern "C" GAME_GET_SOUND_SAMPLES(GameGetSoundSamples)
{
    game_state *gameState = (game_state*)memory->PermanentStorage;      
    GameOutputSound(soundBuffer, gameState, 412);
}
