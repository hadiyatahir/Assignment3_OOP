#pragma once
#ifndef HEADER_H
#define HEADER_H

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ENTITY $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

class Entity {
protected:
	int id;
	char* name;

public:
	Entity();    //defualt for virtual inheritance
	Entity(int id1, char* name);

	~Entity();

};


////////////////////////////////////////////////ASSETS////////////////////////////////////

//child class of entity

//virtual due to warriorlord as entity appears twice in it
class Assets : virtual public Entity {

protected:
	int hp;
	int maxHp;
	int armor;
	int maintenanceCost;

public:
	Assets();
	Assets(int id1, char* name, int h, int m, int a, int cost);

	void setHp(int h);

	int getHp();
};


//child class of assets

class MilitaryUnit : public Assets {
protected:
	int attackPower;

public:
	//adding parameters of both assets and entity (entire chain)
	MilitaryUnit(int id1, char* name, int h, int m, int a, int cost, int attack);

	int getAttackPower();
};


//child class of military unit

class MountedUnit : public MilitaryUnit {
protected:
	float chargeMultiplier;
	float terrainPenalty;

public:
	MountedUnit(int id1, char* name, int h, int m, int a, int cost, int attack, bool front
		, float charge, float penalty);
};


// child class of mounted unit

class Cavalry : public MountedUnit {
	//no additional attributes

public:
	Cavalry(int id1, char* name, int h, int m, int a, int cost, int attack, bool front
		, float charge, float penalty);
};


//child class of military unit

class MeleeUnit : public MilitaryUnit {
protected:
	bool isFrontline;

public:
	MeleeUnit(int id1, char* name, int h, int m, int a, int cost, int attack, bool front);
};


// child class of melee unit

class FootSoldier : public MeleeUnit {

public:
	FootSoldier(int id1, char* name, int h, int m, int a, int cost, int attack, bool front);
};


//child class of melee unit

class Knight : public MeleeUnit {
private:
	int shieldBlock;

public:
	Knight(int id1, char* name, int h, int m, int a, int cost, int attack, bool front, int shield);
};


//child class of assets

class SeigeEngine : virtual public Assets {
protected:
	int seigeDamage;
	int speed;

public:
	/*SeigeEngine(int id1, char* name, int h, int m, int a, int cost, int damage,
		int sp);*/

	SeigeEngine(int damage, int sp);
};


//child class of seige engine

class Catapult : public SeigeEngine {
private:     //last class in heirarchy thus private
	int aoeDamage;
	int aoeTargets;

public:
	Catapult(int id1, char* name, int h, int m, int a, int cost, int damage,
		int sp, int aoedam, int aoetarg);
};


// child class of seige engine

class BatteringRam : public SeigeEngine {
	//no extra attributes

public:
	BatteringRam(int id1, char* name, int h, int m, int a, int cost, int damage,
		int sp);
};


//child class of assets

class Transport : virtual public Assets {
protected:
	int troopCapacity;
	float knotsSpeed;

public:
	Transport(int troop, float knots);
};


//child class of both seige engine and transport

class Warship : public SeigeEngine, public Transport {
public:
	Warship(int id1, char* name, int h, int m, int a, int cost, int damage,
		int sp, int troop, float knots);
};


//////////////////////////////////////////////////////// PERSON ///////////////////////////////////////


//child class of entity

//virtual due to warriorlord as entity appears twice in it
class Person : virtual public Entity {
protected:
	int age;
	bool isAlive;

public:
	Person();
	Person(int id1, char* name, int a, bool alive);

	int getAge();
};


//child class of person

class Noble : public Person {
protected:
	int realmIdx;
	float ambitionStat;
	float diplomacyStat;
	float healthMeter;
	int fatherId;
	int* childrenIds;
	int childCount;

public:
	Noble(int id1, char* name, int a, bool alive, int realmid, float ambition,
		float diplomacy, float health, int father, int* childarr, int numchild);

	int getAmbitionStat();

	~Noble();    //for children id array
};


// child class of noble

class LandedLord : public Noble {
private:
	float strategyStat;

public:
	LandedLord(int id1, char* name, int a, bool alive, int realmid, float ambition,
		float diplomacy, float health, int father, int* childarr, int numchild, float strategy);

	int getStrategyStat();
};


// child class of noble

class LandlessLord : public Noble {
private:
	float plotProgree;   //assassination plot progress

public:
	LandlessLord(int id1, char* name, int a, bool alive, int realmid, float ambition,
		float diplomacy, float health, int father, int* childarr, int numchild, float plot);

	float getPlotProgress();
	void setPlotProgress(float p);
};


//child class of both noble and knight

class WarriorLord : public Noble, public Knight {
public:
	WarriorLord(int id1, char* name, int a, bool alive, int realmid, float ambition,
		float diplomacy, float health, int father, int* childarr, int numchild,
		int h, int m, int a1, int cost, int attack, bool front, int shield);
};


//child class of person

class Sage : public Person {
private:
	float healingPwr;
	float wisdom;

public:
	Sage(int id1, char* name, int a, bool alive, float healing, float wis);
};


//child class of person

class Assassin : public Person {
private:
	float stealthStat;
	int clientIdx;

public:
	Assassin(int id1, char* name, int a, bool alive, float stealth, int clientid);
};





//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ KINGDOM $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

class Kingdom {
protected:

	//crown
	int wealth;
	int defenseStat;

	int* heirIds;
	int heirCount;

	LandlessLord** courtiers;
	int courtierCount;

	LandedLord* currentLord;   //for engine fucntions
	LandlessLord* rebelLord;   //for rebel leader ot be current leader

	//vanguard
	FootSoldier** footSoldiers;
	int footSoldierCount;

	Knight** knights;
	int knightCount;

	Cavalry** cavalry;
	int cavalryCount;

	BatteringRam** batteringRam;
	int batteringRamCount;

	Catapult** catapult;
	int catapultCount;

	Warship** warships;
	int warshipCount;

	WarriorLord** warriorLords;
	int warriorLordCount;

	//shadows
	Sage** sages;
	int sageCount;

	float civilWar;

public:
	Kingdom();

	Kingdom(const Kingdom& other);  //copy contrucotr

	virtual ~Kingdom();

	virtual void performAction();

	Cavalry** getCavalryArray();
	int getCavalryCount();
	void setCavalryCount(int cc);
	void addCavalry(Cavalry* c);
	void removeCavalry(int index);

	Knight** getKnightArray();
	int getKnightCount();
	void setKnightCount(int kc);
	void addKnight(Knight* K);
	void removeKnight(int index);

	FootSoldier** getFootSoldierArray();
	int getFootSoldierCount();
	void setFootSoldierCount(int fc);
	void addFootSoldier(FootSoldier* fs);
	void removeFootSoldier(int index);

	WarriorLord** getWarriorLordArray();
	int getWarriorLordCount();
	void setWarriorLordCount(int wc);
	void addWarrior(WarriorLord* w);
	void removeWarrior(int index);

	bool IsArmyDefeated();

	void CivilWar();

	float getCivilWar();
	void setCivilWar(float cw);

	LandlessLord** getCourtierArray();
	int getCourtierCount();
	void setCourtierCount(int c);

	int getWealth();
	int getDefenseStat();

	void setWealth(int w);
	void setDefenseStat(int ds);

	void setCurrentLord(LandedLord* l);
	LandedLord* getCurrentLord();

	void setRebelLord(LandlessLord* r);
	LandlessLord* getRebelLord();


	void updatePlotProgress(int* relation, int kingCount);
	void solveAssassination(LandlessLord* culprit);

};


//child class of kingdom

class FrostPeaksKingdom : public Kingdom {
protected:
	int coldAttritionModifier;
	void applyColdWeather();

public:
	FrostPeaksKingdom();
	FrostPeaksKingdom(int coldmod);
	void performAction() override;     //overrisidng the base class's fucntion


};

//child class of frost peaks kingdom

class ThePeaksofFrost : public FrostPeaksKingdom {
public:
	ThePeaksofFrost(int coldmod);
	void performAction() override;
};


//child class of kingdom

class TheVerdantKingdom : public Kingdom {
protected:
	int foodSurplusBonus;
	void applyFoodSurplus();

public:
	TheVerdantKingdom(int bonus = 0);
	void performAction() override;
};

//child class of the verdant kingdom

class TheSkyCitadel : public TheVerdantKingdom {
public:
	TheSkyCitadel(int bonus);
	void performAction() override;
};

//child class of the verdant kingdom

class TheVerdantExpanse : public TheVerdantKingdom {
public:
	TheVerdantExpanse(int bonus);
	void performAction() override;
};

//child class of verdant kindgom

class TheCrimsonSands : public TheVerdantKingdom {
public:
	TheCrimsonSands(int bonus);
	void performAction() override;
};

//child class of verdant kingdom

class TheAzureRiverLands : public TheVerdantKingdom {
public:
	TheAzureRiverLands(int bonus);
	void performAction() override;
};


//child class of kingdom

class CoastalKingdom : public Kingdom {
protected:
	int navalTradeBonus;
	void applyTradeBonus();

public:
	CoastalKingdom(int bonus = 0);
	void performAction() override;
};

//child class of coastal kingodm

class TheSunLandEmpire : public CoastalKingdom {
public:
	TheSunLandEmpire(int bonus);
	void performAction() override;
};

//child class of coastal kingdom

class TheCorsairIsles : public CoastalKingdom {
public:
	TheCorsairIsles(int bonus);
	void performAction() override;
};

//child class of coastal kingdom

class TheTempestCoast :public CoastalKingdom {
public:
	TheTempestCoast(int bonus);
	void performAction() override;
};

//child class of coastal kingdom

class TheImperialSeat : public CoastalKingdom {
public:
	TheImperialSeat(int bonus);
	void performAction() override;
};

//child class of kingdom (direct inhertiance)

class TheVoidRift : public Kingdom {
protected:
	int threatLvl;

public:
	TheVoidRift(int threatLvl = 0);
	void performAction() override;
};





//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ENGINE $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

class Kingdom;

class Engine {
private:
	Kingdom** kingdoms;
	int kingdomCount;

	Kingdom** rebelFactions;
	int rebelCount;

	int** relations;

	int totalDays;

public:
	Engine(Kingdom** kingdoms = nullptr, int count = 0, int** relations = nullptr, int total = 0);

	void Simulation();

	void checkForWar();

	void simulateFight(Kingdom* A, Kingdom* B);

	void simulateHour(Kingdom* A, Kingdom* B);

	float calculateDamage(Kingdom* A);    //calculating total attack for this hour

	void applyDamage(Kingdom* A, float damage);

	void triggerWar();

	void ArmySplit(Kingdom* parent, Kingdom* rebel);

	int** getRelations();
};




























#endif // !HEADER_H
