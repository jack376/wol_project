#include "stdafx.h"
#include "SceneGame.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "GameObject.h"
#include "Framework.h"
#include "TextGo.h"
#include "SpriteGo.h"
#include "Monster.h"
#include "Lancer.h"
#include "Archer.h"
#include "Mage.h"
#include "FireBoss.h"
#include "Player.h"
#include "ElementalSpell.h"
#include "Monster.h"
#include "TileInfoTable.h"
#include "rapidcsv.h"
#include "Tile.h"
#include "BoxCollider2D.h"
#include "DecoGo.h"
#include "GlowGo.h"
#include "Particle.h"
#include "Skill.h"
#include "SkillMgr.h"
#include "Slot.h"
#include "MenuInventory.h"
#include "QuickSlot.h"
#include "HPBar.h"

SceneGame::SceneGame() : Scene(SceneId::Game)
{
}

void SceneGame::Init()
{
	Release();

	auto size = FRAMEWORK.GetWindowSize();


	player = (Player*)AddGo(new Player());
	player->SetPosition(700, 700);
	player->sprite.setScale(4, 4);
	player->SetOrigin(Origins::MC);
	player->sortLayer = 5;
	player->SetScene(this);

	// Load Tilemap CSV
	LoadFromCSV("tables/EntireRoom_0825_182721.csv");

	TilesToIntMap();
	CalculatorNongroundTiles();
	CreateMiniMap();

	menu = (MenuInventory*)AddGo(new MenuInventory());
	quickSlot = (QuickSlot*)AddGo(new QuickSlot());
	menu->SetQuickSlot(quickSlot);

	player->SetTiles(&tilesWorld);
	player->SetMonsterList(monsters);

	
	monster = CreateMonster(MonsterId::FireBoss);
	monster->SetPlayer(player);
	monster->SetTiles(&tilesWorld);
	monster->SetIntMap(&intMap);
	monster->SetNonGroundTiles(&nongroundTiles);
	monster->SetPosition(512, 1024);
	monster->particlePool = &this->particlePool;
	monsters.push_back(monster);

	HPBar* hpb = dynamic_cast<HPBar*>(AddGo(new HPBar("BossHP")));
	hpb->SetTarget(monster->GetMaxHP(), monster->GetHP());
	hpb->SetOrigin(Origins::ML);
	hpb->SetPosition(size.x * 0.4, size.y * 0.1);
	hpb->sortLayer = 105;

	hpb = dynamic_cast<HPBar*>(AddGo(new HPBar("PlayerHP")));
	hpb->SetTarget(player->GetMaxHP(), player->GetHP());
	hpb->SetOrigin(Origins::ML);
	hpb->SetPosition(size.x * 0.05, size.y * 0.05);
	hpb->sortLayer = 105;

	monster = CreateMonster(MonsterId::Ghoul);
	monster->SetPlayer(player);
	monster->SetTiles(&tilesWorld);
	monster->SetIntMap(&intMap);
	monster->SetNonGroundTiles(&nongroundTiles);
	monster->SetPosition(700, 500);
	monsters.push_back(monster);

	player->SetTiles(&tilesWorld);
	player->SetMonsterList(monsters);

	//tempWindSlash->SetTiles(&tilesWorld);
	//tempFireBall->SetTiles(&tilesWorld);

	//tempWindSlash->SetMonsterList(monsters);
	//tempFireBall->SetMonsterList(monsters);

	//tempFireBall->SetMonsterList(monsters);

	// Create Particle
	CreateParticle(1000);

	// Spawn Deco
	SpawnDecoGo(100);
	SpawnWallDecoGo(15);
	SpawnGlowGoLarge();
	//SpawnGlowGoSmall();

	for (auto go : gameObjects)
	{
		go->Init();
	}

	SKILL_MGR.SetTiles(&tilesWorld);
	SKILL_MGR.SetMonsterList(monsters);
	SKILL_MGR.SetPlayer(player);
	SKILL_MGR.Init();

	// 스킬 임시 장착 / 스킬 구매하면 Tab메뉴에 생성하고
	// 그 Tab메뉴에서 장착해야 Equip슬롯으로 간다
	// 스킬 장착 이후에 스킬을 슬롯에 적용시켜야한다.

	for (auto skillTable : SKILL_MGR.GetExistSkillList())
	{
		skillTable.second->SetPlayer(player);
		skillTable.second->SetMonsterList(monsters);
		skillTable.second->SetPlayer(player);
		SKILL_MGR.EquipSkill(skillTable.second);
	}

	//for (int i = 0; i < SKILL_MGR.GetExistSkillList().size(); i++)
	//{
	//	Skill* skill = SKILL_MGR.SearchExistedSkill((SkillIds)i);
	//	skill->SetSkillEvent((SkillEvents)i);
	//	skill->SetPlayer(player);
	//	skill->SetMonsterList(monsters);
	//	skill->SetTiles(&tilesWorld);
	//	SKILL_MGR.EquipSkill(skill);
	//}
	// 슬롯 작업
}

void SceneGame::Release()
{
	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;  
	}
}

void SceneGame::Enter()
{
	auto size = FRAMEWORK.GetWindowSize();
	worldView.setSize(size);
	worldView.setCenter(0, 0);

	uiView.setSize(size);
	uiView.setCenter(size * 0.5f);

	miniMapView.setSize(size * 1.2f);
	miniMapView.setViewport(sf::FloatRect(0.0f, 0.1f, 0.2f, 0.2f));

	Scene::Enter();

	ClearObjectPool(particlePool);
}

void SceneGame::Exit()
{
	SKILL_MGR.SaveEquipedSkill();
	Scene::Exit();
}

void SceneGame::Update(float dt)
{
	Scene::Update(dt);	
	debugTimer += dt;
	worldView.setCenter(player->GetPosition());
	miniMapView.setCenter(player->GetPosition());
	CheckMiniMap(11, 22);
	//isCol = colliderManager.ObbCol(monster->rect, tempWindSlash->GetCollider());
	//isCol = colliderManager.ObbCol(tempWindSlash->GetCollider(), monster->rect);

	//if (debugTimer > debugDuration && !isCol)
	//{
	//	debugTimer = 0.f;
	//	std::cout << "OBB is Failed" << std::endl;
	//}
	//if (isCol)
	//{
	//	std::cout << "OBB is Succesd" << std::endl;
	//}
	worldView.setCenter(player->GetPosition());

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Tilde))
	{
		SCENE_MGR.ChangeScene(SceneId::Editor);
	}

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Tab))
	{
		menu->AllSetActive(!menu->GetActive());
		Slot::selectedSlot = nullptr;
		isMenuOn = menu->GetActive();
		std::cout << isMenuOn << std::endl;
	}

	if (!isMenuOn)
	{
		menu->AllSetActive(isMenuOn);
	}


}

template<typename T>
inline void SceneGame::ClearObjectPool(ObjectPool<T>& pool)
{
	for (auto obj : pool.GetUseList())
	{
		RemoveGo(obj);
	}
	pool.Clear();
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);

	window.setView(miniMapView);
	sf::View currentView = window.getView();
	sf::FloatRect viewBounds(currentView.getCenter() - currentView.getSize() / 2.f, currentView.getSize());

	for (auto& row : miniMap)
	{
		for (auto& cell : row)
		{
			if (cell.first->getGlobalBounds().intersects(viewBounds))
				window.draw(*cell.first);
		}
	}
	window.draw(miniMapPlayer);
	
}

Tile* SceneGame::CreateTile(const std::string& name, float posX, float posY, int sort)
{
	Tile* tile = (Tile*)AddWithoutCheckGo(new Tile(name));
	tile->sortLayer = sort;
	tile->SetPosition(posX, posY);

	return tile;
}

void SceneGame::LoadFromCSV(const std::string& path)
{
	rapidcsv::Document doc(path);

	int maxTileIndexX = 0;
	int maxTileIndexY = 0;

	for (size_t i = 0; i < doc.GetRowCount(); i++)
	{
		int tileIndexX = doc.GetCell<int>("tileIndexX", i);
		int tileIndexY = doc.GetCell<int>("tileIndexY", i);

		if (tileIndexX > maxTileIndexX) maxTileIndexX = tileIndexX;
		if (tileIndexY > maxTileIndexY) maxTileIndexY = tileIndexY;
	}
	rows = maxTileIndexX + 1;
	cols = maxTileIndexY + 1;
	CreateTile2dVector(rows, cols);

	for (size_t i = 0; i < doc.GetRowCount(); i++)
	{
		std::string tileName = doc.GetCell<std::string>("tileName", i);

		int tileIndexX = doc.GetCell<int>("tileIndexX", i);
		int tileIndexY = doc.GetCell<int>("tileIndexY", i);
		int tileType   = doc.GetCell<int>("tileType", i);
		int tileSize   = doc.GetCell<int>("tileSize", i);
		int tileScale  = doc.GetCell<int>("tileScale", i);
		int tileLayer  = doc.GetCell<int>("tileLayer", i);

		std::string textureId = doc.GetCell<std::string>("textureId", i);

		sf::IntRect topTextureRect(doc.GetCell<int>("topTextureRectL", i), doc.GetCell<int>("topTextureRectT", i), tileSize, tileSize);
		sf::IntRect bottomTextureRect(doc.GetCell<int>("bottomTextureRectL", i), doc.GetCell<int>("bottomTextureRectT", i), tileSize, tileSize);

		int spawnLocation = doc.GetCell<int>("spawnLocation", i);

		Tile* tile = (Tile*)FindGo(tileName);
		tile->SetIndex(tileIndexX, tileIndexY);
		tile->SetType(static_cast<TileType>(tileType));
		tile->SetTileSize(tileSize);
		tile->SetScale(tileScale);
		tile->SetLayer(tileLayer);
		tile->SetTexture(textureId);
		tile->SetTextureRectTop(topTextureRect, textureId);
		tile->SetTextureRectBottom(bottomTextureRect, textureId);
		tile->SetSpawnLocation(static_cast<SpawnLocation>(spawnLocation));
		tile->SetOrigin(Origins::TL);
		tilesWorld[tileIndexX][tileIndexY] = tile;

		if (tile->GetType() == TileType::None)
		{
			tile->SetActive(false);
		}
	}
	//std::cout << "SYSTEM : Load Success" << std::endl;
}

Monster* SceneGame::CreateMonster(MonsterId id)
{
	Monster* monster = nullptr;
	switch (id)
	{
	case MonsterId::Ghoul:
		monster = dynamic_cast<Monster*>(AddGo(new Monster(id)));
		break;
	case MonsterId::GhoulLarge:
		monster = dynamic_cast<Monster*>(AddGo(new Monster(id)));
		break;
	case MonsterId::Lancer:
		monster = dynamic_cast<Monster*>(AddGo(new Lancer(id)));
		break;
	case MonsterId::Archer:
		monster = dynamic_cast<Monster*>(AddGo(new Archer(id)));
		break;
	case MonsterId::Mage:
		monster = dynamic_cast<Monster*>(AddGo(new Mage(id)));
		break;
	case MonsterId::FireBoss:
		monster = dynamic_cast<Monster*>(AddGo(new FireBoss(id)));
		break;
	}
	return monster;
}

void SceneGame::CreateTile2dVector(int rows, int cols)
{
	tilesWorld.resize(rows, std::vector<Tile*>(cols, nullptr));
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			Tile* tile = CreateTile("Tile(" + std::to_string(i) + ", " + std::to_string(j) + ")", i * tileSize, j * tileSize);
			tilesWorld[i][j] = tile;
		}
	}
}

void SceneGame::CreateParticle(int count)
{
	particlePool.OnCreate = [this](Particle* particle)
	{
		int rand = 16 * Utils::RandomRange(0, 8);

		particle->SetName("Particle");
		particle->SetDuration(1.0f);
		particle->SetTexture("graphics/game/Fire.png");
		particle->SetTextureRect(sf::IntRect(rand, 0, 16, 16));
		particle->sortLayer = 25;
		particle->sortOrder = 0;
		particle->SetPool(&particlePool);
		particle->Reset();
		particle->SetActive(false);
	};
	particlePool.Init(count);
}

void SceneGame::SpawnDecoGo(int count)
{
	std::vector<sf::Vector2f> objectSpawnArea;
	
	for (size_t row = 0; row < tilesWorld.size(); row++)
	{
		for (size_t col = 0; col < tilesWorld[row].size(); col++)
		{
			if (tilesWorld[row][col]->GetSpawnLocation() == SpawnLocation::Object)
			{
				objectSpawnArea.push_back(tilesWorld[row][col]->GetPosition());
			}
		}
	}

	if (objectSpawnArea.empty())
	{ 
		return; 
	}

	for (int i = 0; i < count && !objectSpawnArea.empty(); i++)
	{
		int randomIndex = Utils::RandomRange(0, objectSpawnArea.size() - 1);
		int randomNumber = Utils::RandomRange(0, 11);
		sf::Vector2f randomPosition = objectSpawnArea[randomIndex];

		objectSpawnArea.erase(objectSpawnArea.begin() + randomIndex);

		DecoGo* object = (DecoGo*)AddGo(new DecoGo("graphics/editor/FireTileSet.png", "Object" + i));
		object->sortLayer = 3;
		object->SetActive(true);
		object->SetPlayer(player);
		object->SetPosition(randomPosition);
		object->SetTextureRect(sf::IntRect(304 + (randomNumber * 16), 384, 16, 32));
		object->SetScale(4.0f, 4.0f);
		object->SetParticlePool(&particlePool);
		object->SetOrigin(Origins::TL);
		object->Reset();
	}
}

void SceneGame::SpawnWallDecoGo(int count)
{
	std::vector<sf::Vector2f> objectSpawnArea;

	for (size_t row = 0; row < tilesWorld.size(); row++)
	{
		for (size_t col = 0; col < tilesWorld[row].size(); col++)
		{
			if (tilesWorld[row][col]->GetSpawnLocation() == SpawnLocation::WallObject)
			{
				objectSpawnArea.push_back(tilesWorld[row][col]->GetPosition());
			}
		}
	}

	if (objectSpawnArea.empty())
	{
		return;
	}

	for (int i = 0; i < count && !objectSpawnArea.empty(); i++)
	{
		int randomIndex = Utils::RandomRange(0, objectSpawnArea.size() - 1);
		int randomNumber = Utils::RandomRange(0, 2);
		sf::Vector2f randomPosition = objectSpawnArea[randomIndex];

		objectSpawnArea.erase(objectSpawnArea.begin() + randomIndex);

		DecoGo* object = (DecoGo*)AddGo(new DecoGo("graphics/editor/FireTileSet.png", "WallObject" + i));
		object->sortLayer = 4;
		object->SetActive(true);
		object->SetPlayer(player);
		object->SetPosition(randomPosition);
		object->SetTextureRect(sf::IntRect(240 + (randomNumber * 32), 288, 32, 32));
		object->SetScale(4.0f, 4.0f);
		object->SetParticlePool(&particlePool);
		object->SetOrigin(Origins::TL);
		object->Reset();
	}
}

void SceneGame::SpawnGlowGoLarge()
{
	std::vector<sf::Vector2f> objectSpawnArea;

	for (size_t row = 0; row < tilesWorld.size(); row++)
	{
		for (size_t col = 0; col < tilesWorld[row].size(); col++)
		{
			if (tilesWorld[row][col]->GetSpawnLocation() == SpawnLocation::FireLight)
			{
				objectSpawnArea.push_back(tilesWorld[row][col]->GetPosition());
			}
		}
	}

	if (objectSpawnArea.empty())
	{
		return;
	}

	for (int i = 0; i < objectSpawnArea.size() && !objectSpawnArea.empty(); i++)
	{
		GlowGo* object = (GlowGo*)AddGo(new GlowGo("graphics/effect/Glow.png", "GlowL" + i));
		object->sortLayer = 10;
		object->SetActive(true);
		object->SetPlayer(player);
		object->SetPosition(objectSpawnArea[i]);
	}
}

void SceneGame::SpawnGlowGoSmall()
{
	std::vector<sf::Vector2f> objectSpawnArea;

	for (size_t row = 0; row < tilesWorld.size(); row++)
	{
		for (size_t col = 0; col < tilesWorld[row].size(); col++)
		{
			if (tilesWorld[row][col]->GetSpawnLocation() == SpawnLocation::Embers);
			{
				objectSpawnArea.push_back(tilesWorld[row][col]->GetPosition());
			}
		}
	}

	if (objectSpawnArea.empty())
	{
		return;
	}

	for (int i = 0; i < objectSpawnArea.size() && !objectSpawnArea.empty(); i++)
	{
		GlowGo* object = (GlowGo*)AddGo(new GlowGo("graphics/effect/Glow.png", "GlowS" + i));
		object->sortLayer = 10;
		object->SetActive(true);
		object->SetPlayer(player);
		object->SetScale(0.66f, 0.66f);
		object->SetPosition(objectSpawnArea[i]);
	}
}

void SceneGame::DestroyDecoGo(DecoGo* obj)
{
	RemoveGo(obj);
	count++; // 임시
}

void SceneGame::TilesToIntMap()
{
	for (auto& tiles : tilesWorld)
	{
		std::vector<int> row;
		for (auto& tile : tiles)
		{
			if (tile->GetType() == TileType::Ground)
				row.push_back(0);
			else
				row.push_back(-1);
		}
		intMap.push_back(row);
	}
}

void SceneGame::CalculatorNongroundTiles()
{
	nongroundTiles.clear();

	for (auto& row : tilesWorld)
	{
		for (auto& tile : row)
		{
			if (tile->GetType() != TileType::Ground)
				nongroundTiles.push_back(tile);
		}
	}
}

void SceneGame::CreateMiniMap()
{
	miniMapPlayer.setFillColor(sf::Color::Green);
	miniMapPlayer.setSize({ 64, 64 });
	for (auto& tiles : tilesWorld)
	{
		std::vector<std::pair<sf::RectangleShape*, bool>> row;
		for (auto& tile : tiles)
		{
			if (tile->GetType() == TileType::Ground)
			{
				sf::RectangleShape* rectangle = new sf::RectangleShape({ 64, 64 });
				rectangle->setFillColor(sf::Color::White);
				sf::Color color = rectangle->getFillColor();
				color.a = 0;
				rectangle->setFillColor(color);
				rectangle->setPosition(tile->GetPosition());
				row.push_back(std::pair<sf::RectangleShape*, bool>(rectangle, false));
			}	
			else
			{
				sf::RectangleShape* rectangle = new sf::RectangleShape({ 64, 64 });
				rectangle->setFillColor({128, 128, 128});
				sf::Color color = rectangle->getFillColor();
				color.a = 0;
				rectangle->setFillColor(color);
				rectangle->setPosition(tile->GetPosition());
				row.push_back(std::pair<sf::RectangleShape*, bool>(rectangle, false));
			}	
		}
		miniMap.push_back(row);
	}
}

std::vector<sf::RectangleShape*> SceneGame::CheckMiniMap(int row, int col)
{
	int searchRowRange = row;
	int searchColRange = col;

	sf::Vector2i index = player->GetCurrentTile()->GetIndex();
	std::vector<sf::RectangleShape*> tiles;

	int topRowIndex = index.x - searchRowRange < 0 ? 0 : index.x - searchRowRange;
	int leftColumnIndex = index.y - searchColRange < 0 ? 0 : index.y - searchColRange;
	int bottomRowIndex = index.x + searchRowRange >= miniMap.size() ? miniMap.size() - 1 : index.x + searchRowRange;
	int rightColumnIndex = index.y + searchColRange >= miniMap[0].size() ? miniMap[0].size() - 1 : index.y + searchColRange;

	miniMapPlayer.setPosition(miniMap[index.x][index.y].first->getPosition());

	for (int i = topRowIndex; i < bottomRowIndex; i++)
	{
		for (int j = leftColumnIndex; j < rightColumnIndex; j++)
		{
			miniMap[i][j].second = true;
			
			if (intMap[i][j] == 0)
			{
				sf::Color color = sf::Color::White;
				color.a = 255;
				miniMap[i][j].first->setFillColor(color);
			}
			else
			{
				sf::Color color = {128, 128, 128};
				color.a = 100;
				miniMap[i][j].first->setFillColor(color);
			}
			tiles.push_back(miniMap[i][j].first);
		}
	}
	return tiles;
}
