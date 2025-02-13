#ifndef SPRITEANIM_H
#define SPRITEANIM_H

#include "Resource.h"

class SpriteAnim : Resource
{
public:
	//Constructors/ Destructors
	SpriteAnim();
	virtual ~SpriteAnim();

	//Accessors
	short GetClipCurrent() { return (short)m_clipCurrent; }

	//Methods
	void Serialize(std::ostream& _stream) override;
	void Deserialize(std::istream& _stream) override;
	void ToString() override; 

	void ClearMemory();
	void Create(short _clipStart, short _clipCount, float _clipSpeed);
	void Update();

	//Members
	static ObjectPool<SpriteAnim>* Pool;

private:
	//Members
	short m_clipStart;
	short m_clipCount;
	short m_clipEnd;
	float m_clipCurrent;
	float m_clipSpeed;
};

#endif // SPRITEANIM_H



