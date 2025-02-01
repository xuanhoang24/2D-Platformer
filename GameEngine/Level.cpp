#include "Level.h"

Level::Level()
{
	AssetController::Instance().Initialize(10000000); //Alocate 10MB
	Unit::Pool = new ObjectPool<Unit>();
	SoundEffect::Pool = new ObjectPool<SoundEffect>();

	m_mapSizeX = 0;
	m_mapSizeY = 0;
	m_gameTime = 0.0f;
	m_unit = nullptr;
}

Level::~Level()
{
	delete SoundEffect::Pool;
	delete Unit::Pool;
	AssetController::Instance().Clear(); //Free 10MB
}

void Level::AssignNonDefaultValues()
{
	m_mapSizeX = 128;
	m_mapSizeY = 256;
	m_gameTime = 101.5f;
	m_unit = Unit::Pool->GetResource();
	m_unit->AssignNonDefaultValues();
	Resource::AssignNonDefaultValues();
}

void Level::Serialize(std::ostream& _stream)
{
	_stream.write(reinterpret_cast<char*>(&m_mapSizeX), sizeof(m_mapSizeX));
	_stream.write(reinterpret_cast<char*>(&m_mapSizeY), sizeof(m_mapSizeY));
	_stream.write(reinterpret_cast<char*>(&m_gameTime), sizeof(m_gameTime));
	SerializePointer(_stream, m_unit);
	Resource::Serialize(_stream);
}

void Level::Deserialize(std::istream& _stream)
{
	_stream.read(reinterpret_cast<char*>(&m_mapSizeX), sizeof(m_mapSizeX));
	_stream.read(reinterpret_cast<char*>(&m_mapSizeY), sizeof(m_mapSizeY));
	_stream.read(reinterpret_cast<char*>(&m_gameTime), sizeof(m_gameTime));
	DeserializePointer(_stream, m_unit);
	Resource::Deserialize(_stream);
}

void Level::ToString()
{
	cout << "LEVEL" << endl;
	cout << "MapSizeX: " << m_mapSizeX << endl;
	cout << "MapSizeY: " << m_mapSizeY << endl;
	cout << "GameTime: " << m_gameTime << endl;
	m_unit->ToString();
	Resource::ToString();
}