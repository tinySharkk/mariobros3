#include "Map.h"
#include "Ground.h"
#include "GhostPlatform.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Brick.h"
#include "QuestionBlock.h"
#include "Coin.h"
#include "Deadblock.h"
#include "RedFireShootingPlant.h"
#include "GreenFireShootingPlant.h"
#include "GreenEatingPlant.h"
#include "Warp.h"
#include "PSPortal.h"
#include "BrickQBlock.h"
#include "EndGameReward.h"
#include "Mario.h"
#include "Bush.h"
#include "NodeMap.h"
#include "Node.h"
#include "Scence.h"
#include "WorldMapScene.h"
#include "Curtain.h"
#include "RedKoopas.h"
#include "MusicNode.h"
#include "RedMusicNode.h"
#include "ParaGoomba.h"
#include "FlyKoopas.h"
#include "Pipe.h"
#include "BoomerangBro.h"
#include "FlyGoomba.h"
Map::Map()
{
	this->tileHeight = 0;
	this->tileWidth = 0;
	this->mapWidth = 0;
	this->mapHeight = 0;
	this->tileSets = 0;
}

Map::~Map()
{
}

void Map::AddTileSet(LPTILESET tileset)
{
	this->tileSets = tileset;
}

void Map::AddTileMap(LPTILEMAP tilemap)
{
	this->tileMaps.push_back(tilemap);
}

void Map::LoadMap(const char* filePath, const char* path)
{
	TiXmlDocument TMXdoc(filePath);

	if (TMXdoc.LoadFile())
	{
		TiXmlElement* root = TMXdoc.RootElement();

		root->QueryIntAttribute("width", &this->mapWidth);
		root->QueryIntAttribute("height", &this->mapHeight);
		root->QueryIntAttribute("tilewidth", &this->tileWidth);
		root->QueryIntAttribute("tileheight", &this->tileHeight);

		for (TiXmlElement* TMXtileset = root->FirstChildElement("tileset"); TMXtileset != NULL; TMXtileset = TMXtileset->NextSiblingElement("tileset"))
		{
			LPTILESET tileset = new TileSet(TMXtileset, path);
			this->AddTileSet(tileset);

			//DebugOut(L"[INFO] load success tileset: %", tileset->getFirstgid());
		}

		for (TiXmlElement* TMXlayer = root->FirstChildElement("layer"); TMXlayer != NULL; TMXlayer = TMXlayer->NextSiblingElement("layer"))
		{
			LPTILEMAP tilemap = new TileMap(TMXlayer);
			this->AddTileMap(tilemap);
		}

		this->AddObject(root);

		DebugOut(L"[INFO] map load successful \n");
	}
	else
	{
		DebugOut(L"[ERROR] failed to load map \n");
	}
}


void Map::AddObject(TiXmlElement* RootElement)
{
	for (TiXmlElement* TMXObjectsgroup = RootElement->FirstChildElement("objectgroup"); TMXObjectsgroup != NULL; TMXObjectsgroup = TMXObjectsgroup->NextSiblingElement("objectgroup"))
	{
		NodeMap* nodemap = new NodeMap();
		for (TiXmlElement* TMXObject = TMXObjectsgroup->FirstChildElement("object"); TMXObject != NULL; TMXObject = TMXObject->NextSiblingElement("object"))
		{
			int id;
			float x, y, width, height;
			std::string name = TMXObject->Attribute("name");
			if (name == "ground")
			{
				TMXObject->QueryIntAttribute("id", &id);
				TMXObject->QueryFloatAttribute("x", &x);
				TMXObject->QueryFloatAttribute("y", &y);
				TMXObject->QueryFloatAttribute("width", &width);
				TMXObject->QueryFloatAttribute("height", &height);
				Ground* ground = new Ground((int)width,(int) height);
				ground->SetPosition(x, y);
				ground->setId(id);
				CGame::GetInstance()->GetCurrentScene()->AddObject(ground);
				DebugOut(L"[INFO] map object ground %f \n", x); 

			}
			if (name == "pipe")
			{
				TMXObject->QueryIntAttribute("id", &id);
				TMXObject->QueryFloatAttribute("x", &x);
				TMXObject->QueryFloatAttribute("y", &y);
				TMXObject->QueryFloatAttribute("width", &width);
				TMXObject->QueryFloatAttribute("height", &height);
				Pipe* pipe = new Pipe((int)width, (int)height);
				pipe->SetPosition(x, y);
				pipe->setId(id);
				std::string type;
				type = TMXObject->Attribute("type");
				if (type == "green")
				{
					pipe->SetPipeType(PipeType::green);
				}
				else if (type == "black")
				{
					pipe->SetPipeType(PipeType::black);
				}

				bool isUp;

				TiXmlElement* TMXproperties = TMXObject->FirstChildElement("properties");

				if (TMXproperties != nullptr)
				{
					for (TiXmlElement* TMXproperty = TMXproperties->FirstChildElement("property"); TMXproperty != NULL; TMXproperty = TMXproperty->NextSiblingElement("property"))
					{
						std::string propertyname = TMXproperty->Attribute("name");

						if (propertyname == "isUp")
						{
							TMXproperty->QueryBoolAttribute("value", &isUp);
						}
					}
				}

				pipe->isDirectionUp(isUp);
				CGame::GetInstance()->GetCurrentScene()->AddObject(pipe);

			}
			else if (name == "deadblock")
			{
				TMXObject->QueryIntAttribute("id", &id);
				TMXObject->QueryFloatAttribute("x", &x);
				TMXObject->QueryFloatAttribute("y", &y);
				TMXObject->QueryFloatAttribute("width", &width);
				TMXObject->QueryFloatAttribute("height", &height);
				Deadblock* deadblock = new Deadblock((int)width, (int)height);
				deadblock->SetPosition(x, y);
				deadblock->setId(id);
				CGame::GetInstance()->GetCurrentScene()->AddObject(deadblock);
				//DebugOut(L"[INFO] map object ground \n");

			}
			else if (name == "ghostplatform")
			{
				TMXObject->QueryIntAttribute("id", &id);
				TMXObject->QueryFloatAttribute("x", &x);
				TMXObject->QueryFloatAttribute("y", &y);
				TMXObject->QueryFloatAttribute("width", &width);
				TMXObject->QueryFloatAttribute("height", &height);
				GhostPlatform* ghost = new GhostPlatform((int)width, (int)height);
				ghost->SetPosition(x, y);
				ghost->setId(id);
				CGame::GetInstance()->GetCurrentScene()->AddObject(ghost);
				//DebugOut(L"[INFO] map object ghostplatform \n");
			}
			else if (name == "mario")
			{
				TMXObject->QueryIntAttribute("id", &id);
				TMXObject->QueryFloatAttribute("x", &x);
				TMXObject->QueryFloatAttribute("y", &y);
				__Mario->SetPosition(x, y);
				
			}
			else if (name == "bush")
			{
				TMXObject->QueryIntAttribute("id", &id);
				TMXObject->QueryFloatAttribute("x", &x);
				TMXObject->QueryFloatAttribute("y", &y);
				Bush* bush = new Bush();
				bush->SetPosition(x, y);
				bush->setId(id);
				CGame::GetInstance()->GetCurrentScene()->AddObject(bush);
			}
			else if (name == "goomba")
			{
				TMXObject->QueryIntAttribute("id", &id);
				TMXObject->QueryFloatAttribute("x", &x);
				TMXObject->QueryFloatAttribute("y", &y);
				CGoomba* goomba = new CGoomba();
				goomba->SetPosition(x, y);
				goomba->setId(id);
				CGame::GetInstance()->GetCurrentScene()->AddObject(goomba);
			}
			else if (name == "paragoomba")
			{
				TMXObject->QueryIntAttribute("id", &id);
				TMXObject->QueryFloatAttribute("x", &x);
				TMXObject->QueryFloatAttribute("y", &y);
				ParaGoomba* goomba = new ParaGoomba();
				goomba->SetPosition(x, y);
				goomba->setId(id);
				CGame::GetInstance()->GetCurrentScene()->AddObject(goomba);
			}
			else if (name == "flygoomba")
			{
				TMXObject->QueryIntAttribute("id", &id);
				TMXObject->QueryFloatAttribute("x", &x);
				TMXObject->QueryFloatAttribute("y", &y);
				FlyGoomba* goomba = new FlyGoomba();
				goomba->SetPosition(x, y);
				goomba->setId(id);
				CGame::GetInstance()->GetCurrentScene()->AddObject(goomba);
			}
			else if (name == "redfiretree")
			{
				TMXObject->QueryIntAttribute("id", &id);
				TMXObject->QueryFloatAttribute("x", &x);
				TMXObject->QueryFloatAttribute("y", &y);
				RedFireShootingPlant* obj = new RedFireShootingPlant();
				obj->SetPosition(x, y);
				obj->setId(id);
				CGame::GetInstance()->GetCurrentScene()->AddObject(obj);
			}
			else if (name == "greenfiretree")
			{
				TMXObject->QueryIntAttribute("id", &id);
				TMXObject->QueryFloatAttribute("x", &x);
				TMXObject->QueryFloatAttribute("y", &y);
				GreenFireShootingPlant* greenshoot = new GreenFireShootingPlant();
				greenshoot->SetPosition(x, y);
				greenshoot->setId(id);
				CGame::GetInstance()->GetCurrentScene()->AddObject(greenshoot);
			}
			else if (name == "greeneating")
			{
				TMXObject->QueryIntAttribute("id", &id);
				TMXObject->QueryFloatAttribute("x", &x);
				TMXObject->QueryFloatAttribute("y", &y);
				GreenEatingPlant* obj = new GreenEatingPlant();
				obj->SetPosition(x, y);
				obj->setId(id);
				CGame::GetInstance()->GetCurrentScene()->AddObject(obj);
				DebugOut(L"[INFO] greeneating \n");
			}
			else if (name == "redkoopa")
			{
				TMXObject->QueryIntAttribute("id", &id);
				TMXObject->QueryFloatAttribute("x", &x);
				TMXObject->QueryFloatAttribute("y", &y);
				RedKoopas* koopa = new RedKoopas();
				koopa->SetPosition(x, y);
				koopa->setId(id);
				CGame::GetInstance()->GetCurrentScene()->AddObject(koopa);
			}
			else if (name == "koopa")
			{
				TMXObject->QueryIntAttribute("id", &id);
				TMXObject->QueryFloatAttribute("x", &x);
				TMXObject->QueryFloatAttribute("y", &y);
				CKoopas* koopa = new CKoopas();
				koopa->SetPosition(x, y);
				koopa->setId(id);
				CGame::GetInstance()->GetCurrentScene()->AddObject(koopa);
			}
			else if (name == "flykoopa")
			{
				TMXObject->QueryIntAttribute("id", &id);
				TMXObject->QueryFloatAttribute("x", &x);
				TMXObject->QueryFloatAttribute("y", &y);
				FlyKoopas* koopa = new FlyKoopas();
				koopa->SetPosition(x, y);
				koopa->setId(id);
				CGame::GetInstance()->GetCurrentScene()->AddObject(koopa);
			}
			else if (name == "bbro")
			{
				TMXObject->QueryIntAttribute("id", &id);
				TMXObject->QueryFloatAttribute("x", &x);
				TMXObject->QueryFloatAttribute("y", &y);
				BoomerangBro* bbro = new BoomerangBro();
				bbro->SetPosition(x, y);
				bbro->setId(id);
				CGame::GetInstance()->GetCurrentScene()->AddObject(bbro);
			}
			else if (name == "brick")
			{
				TMXObject->QueryIntAttribute("id", &id);
				TMXObject->QueryFloatAttribute("x", &x);
				TMXObject->QueryFloatAttribute("y", &y);
				CBrick* brick = new CBrick();
				brick->SetPosition(x, y);
				brick->setId(id);
				CGame::GetInstance()->GetCurrentScene()->AddObject(brick);
			}
			else if (name == "music")
			{
				TMXObject->QueryIntAttribute("id", &id);
				TMXObject->QueryFloatAttribute("x", &x);
				TMXObject->QueryFloatAttribute("y", &y);
				MusicNode* brick = new MusicNode();
				brick->SetPosition(x, y);
				brick->setId(id);
				CGame::GetInstance()->GetCurrentScene()->AddObject(brick);
			}
			else if (name == "hiddenmusic")
			{
				TMXObject->QueryIntAttribute("id", &id);
				TMXObject->QueryFloatAttribute("x", &x);
				TMXObject->QueryFloatAttribute("y", &y);
				RedMusicNode* brick = new RedMusicNode();
				brick->SetPosition(x, y);
				brick->setId(id);
				CGame::GetInstance()->GetCurrentScene()->AddObject(brick);
			}
			else if (name == "qblock")
			{
				TMXObject->QueryIntAttribute("id", &id);
				TMXObject->QueryFloatAttribute("x", &x);
				TMXObject->QueryFloatAttribute("y", &y);
				QuestionBlock* qblock = new QuestionBlock();
				qblock->SetPosition(x, y);
				qblock->setId(id);
				CGame::GetInstance()->GetCurrentScene()->AddObject(qblock);
				qblock->SetItem(TMXObject->Attribute("type"));
			}
			else if (name == "bqblock")
			{
				TMXObject->QueryIntAttribute("id", &id);
				TMXObject->QueryFloatAttribute("x", &x);
				TMXObject->QueryFloatAttribute("y", &y);
				BrickQBlock* qblock = new BrickQBlock();
				qblock->SetPosition(x, y);
				qblock->setId(id);
				CGame::GetInstance()->GetCurrentScene()->AddObject(qblock);
				qblock->SetItem(TMXObject->Attribute("type"));
			}
			else if (name == "coin")
			{
				TMXObject->QueryIntAttribute("id", &id);
				TMXObject->QueryFloatAttribute("x", &x);
				TMXObject->QueryFloatAttribute("y", &y);
				Coin* coin = new Coin();
				coin->SetPosition(x, y);
				coin->setId(id);
				CGame::GetInstance()->GetCurrentScene()->AddObject(coin);
			}
			else if (name == "endgamereward")
			{
				TMXObject->QueryIntAttribute("id", &id);
				TMXObject->QueryFloatAttribute("x", &x);
				TMXObject->QueryFloatAttribute("y", &y);
				EndGameReward* reward = new EndGameReward();
				reward->SetPosition(x+9, y+9);
				reward->setId(id);
				CGame::GetInstance()->GetCurrentScene()->AddObject(reward);
			}
			if (name == "curtain")
			{
				TMXObject->QueryIntAttribute("id", &id);
				TMXObject->QueryFloatAttribute("x", &x);
				TMXObject->QueryFloatAttribute("y", &y);
				Curtain* curtain = new Curtain();
				curtain->SetPosition(x, y);
				curtain->setId(id);
				CGame::GetInstance()->GetCurrentScene()->AddObject(curtain);

			}
			else if (name == "node")
			{
				int nodename = 0;
				std::vector<std::string> adlist;
				std::vector<std::string> weight;
				int scence=0;
				TMXObject->QueryIntAttribute("id", &id);
				TMXObject->QueryFloatAttribute("x", &x);
				TMXObject->QueryFloatAttribute("y", &y);
				TMXObject->QueryFloatAttribute("width", &width);
				TMXObject->QueryFloatAttribute("height", &height);
				TiXmlElement* TMXproperties = TMXObject->FirstChildElement("properties");

				if (TMXproperties != nullptr)
				{
					for (TiXmlElement* TMXproperty = TMXproperties->FirstChildElement("property"); TMXproperty != NULL; TMXproperty = TMXproperty->NextSiblingElement("property"))
					{
						std::string propertyname = TMXproperty->Attribute("name");

						if (propertyname == "adjacent_list")
						{
							adlist = split(TMXproperty->Attribute("value"), ",");
						}
						else if (propertyname == "adjacent_weight")
						{
							weight = split(TMXproperty->Attribute("value"), ",");
						}
						else if (propertyname == "node_id")
						{
							nodename = std::stoi(TMXproperty->Attribute("value"));
						}
						else if (propertyname == "scene")
						{
							scence = std::stoi(TMXproperty->Attribute("value"));
						}
					}
				}
				Node* node = new Node();
				node->SetPosition(x, y);
				node->setNodeID(nodename);
				node->setScence(scence);
				node->setId(id);
				auto adjacentlist = node->getAdjacentList();

				for (size_t  i = 0; i < adlist.size(); i++)
				{
					auto id = std::stoi(adlist[i]);

					Edge edge;
					edge.nodeID = id;

					NextNode next = NextNode::none;

					if (weight[i] == "l") next = NextNode::left;
					else if (weight[i] == "r") next = NextNode::right;
					else if (weight[i] == "u") next = NextNode::up;
					else if (weight[i] == "d") next = NextNode::down;
	
					edge.direction = next;

					adjacentlist->push_back(edge);
				}

				nodemap->InsertNode(node);
				
			}
			else if (name == "wrap")
			{
				TMXObject->QueryIntAttribute("id", &id);
				TMXObject->QueryFloatAttribute("x", &x);
				TMXObject->QueryFloatAttribute("y", &y);
				Warp* warp = new Warp();
				warp->SetPosition(x, y);
				warp->setId(id);
				CGame::GetInstance()->GetCurrentScene()->AddObject(warp);
				std::string dir;
				dir = TMXObject->Attribute("type");
				if (dir == "up")
				{
					warp->setWarpDirection(WarpDirection::up);
				}
				else if (dir == "down")
				{
					warp->setWarpDirection(WarpDirection::down);
				}
			}
			else if (name == "psportal")
			{
				TMXObject->QueryIntAttribute("id", &id);
				TMXObject->QueryFloatAttribute("x", &x);
				TMXObject->QueryFloatAttribute("y", &y);
				TMXObject->QueryFloatAttribute("width", &width);
				TMXObject->QueryFloatAttribute("height", &height);
				PSPortal* portal = new PSPortal((int)width,(int) height);
				portal->SetPosition(x, y);
				portal->setId(id);

				bool isstatic = false, isfollow = false, scrollx = false, scrolly = false;
				float camL =0, camR=0, camT=0, camB=0;
				float posX=0, posY=0;

				TiXmlElement* TMXproperties = TMXObject->FirstChildElement("properties");

				if (TMXproperties != nullptr)
				{
					for (TiXmlElement* TMXproperty = TMXproperties->FirstChildElement("property"); TMXproperty != NULL; TMXproperty = TMXproperty->NextSiblingElement("property"))
					{


						std::string propertyname = TMXproperty->Attribute("name");
					 
						if (propertyname == "limitL")
						{
							TMXproperty->QueryFloatAttribute("value", &camL);
						}
						if (propertyname == "limitT")
						{
							TMXproperty->QueryFloatAttribute("value", &camT);
						}
						if (propertyname == "limitR")
						{
							TMXproperty->QueryFloatAttribute("value", &camR);
						}
						if (propertyname == "limitB")
						{
							TMXproperty->QueryFloatAttribute("value", &camB);
						}
						/*if (propertyname == "scrollx")
						{
							TMXproperty->QueryBoolAttribute("value", &scrollx);
						}
						if (propertyname == "scrolly")
						{
							TMXproperty->QueryBoolAttribute("value", &scrolly);
						}*/
						if (propertyname == "isstatic")
						{
							TMXproperty->QueryBoolAttribute("value", &isstatic);
						}
						if (propertyname == "isfollow")
						{
							TMXproperty->QueryBoolAttribute("value", &isfollow);
						}

						if (propertyname == "posx")
						{
							TMXproperty->QueryFloatAttribute("value", &posX);
						}
						if (propertyname == "posy")
						{
							TMXproperty->QueryFloatAttribute("value", &posY);
						}
					}
				}
				portal->isStatic = isstatic;
				portal->isFolow = isfollow;
				portal->setcam(camL, camT, camR, camB);
				portal->setpos(posX, posY);

				CGame::GetInstance()->GetCurrentScene()->AddObject(portal);
			}
		}
		CScene* scence = CGame::GetInstance()->GetCurrentScene();
		if (scence != NULL)
		{
			auto mapscence = dynamic_cast<WorldMapScene*>(scence);
			if (mapscence != NULL)
			{
				mapscence->GetPlayer()->setMap(nodemap);
			}
		}
	}
}

void Map::Render(Camera* camera)
{
	D3DXVECTOR2 direction = D3DXVECTOR2(1.0f, 1.0f);
	int tileX = camera->GetCamPosX() / 48;
	int tileY = camera->GetCamPosY() / 48;
	int tileX_max = (camera->GetCamPosX() + CAMERA_WIDTH ) / 48 +1 ;
	if (tileX_max > mapWidth)
		tileX_max = mapWidth;

	int tileY_max = (camera->GetCamPosY() + CAMERA_HEIGHT) / 48 +1 ;
	for (int i = tileX; i < tileX_max; i++)
	{
		for (int j = tileY; j < tileY_max; j++)
		{
			int x = i * tileWidth;
			int y = j * tileHeight;
			for (LPTILEMAP tile : tileMaps)
			{
				int id = tile->GetMapData(i, j);
				if (id != 0)
				{
					tileSets->Draw(id, (float)x,(float) y);
				}
			}
		}
	}
	//DebugOut(L"[INFO] RENDER MAPP \n");

}
