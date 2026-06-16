#include "q2.h"



//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ENTITY $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

Entity::Entity() : id(0), name(nullptr) {}

Entity::Entity(int id1, char* name) :id(id1) {
	int l = 0;

	while (name[l] != '\0') {
		l++;
	}

	this->name = new char[l + 1];

	for (int i = 0; i < l; i++) {
		this->name[i] = name[i];
	}

	this->name[l] = '\0';


}

Entity::~Entity() {
	delete[] name;
}


////////////////////////////////////////ASSETS//////////////////////////

//======================================ASSETS=========================================

//default constructor due to virtual inheritance issue
Assets::Assets() {
	hp = 0;
	maxHp = 0;
	armor = 0;
	maintenanceCost = 0;
}


Assets::Assets(int id1, char* name, int h, int m, int a, int cost) : Entity(id1, name), hp(h),
maxHp(m), armor(a), maintenanceCost(m) {
}

void Assets::setHp(int h) {
	this->hp = h;
}

int Assets::getHp() {
	return hp;
}

//=============================================MILITARY UNIT===========================================

MilitaryUnit::MilitaryUnit(int id1, char* name, int h, int m, int a, int cost, int attack) :
	Assets(id1, name, h, m, a, cost), attackPower(attack) {
}

int MilitaryUnit::getAttackPower() {
	return attackPower;
}

//=============================================MOUNTED UNIT==========================================

MountedUnit::MountedUnit(int id1, char* name, int h, int m, int a, int cost, int attack, bool front
	, float charge, float penalty) :MilitaryUnit(id1, name, h, m, a, cost,
		attack) {
	this->chargeMultiplier = charge;
	this->terrainPenalty = penalty;
}

//==============================================CAVALARY=======================================

Cavalry::Cavalry(int id1, char* name, int h, int m, int a, int cost, int attack, bool front
	, float charge, float penalty) :MountedUnit(id1, name, h, m, a, cost, attack, front
		, charge, penalty) {
}

//===================================================MELEE UNIT=======================================

MeleeUnit::MeleeUnit(int id1, char* name, int h, int m, int a, int cost, int attack, bool front) :
	MilitaryUnit(id1, name, h, m, a, cost, attack),
	isFrontline(front) {
}

//====================================================FOOT SOLDIER================================

FootSoldier::FootSoldier(int id1, char* name, int h, int m, int a, int cost, int attack, bool front) :
	MeleeUnit(id1, name, h, m, a, cost, attack, front) {
}

//===================================================KNIGHT=============================

Knight::Knight(int id1, char* name, int h, int m, int a, int cost, int attack, bool front, int shield) :
	MeleeUnit(id1, name, h, m, a, cost, attack, front), shieldBlock(shield) {
}

//=================================================SEIGE ENGINE========================================

//SeigeEngine::SeigeEngine(int id1, char* name, int h, int m, int a, int cost, int damage,
//	int sp): Assets(id1, name, h, m, a, cost), seigeDamage(damage),
//	speed(sp){ }

SeigeEngine::SeigeEngine(int damage, int sp) :
	seigeDamage(damage), speed(sp) {
}

//===============================CATAPULT=================================

Catapult::Catapult(int id1, char* name, int h, int m, int a, int cost, int damage,
	int sp, int aoedam, int aoetarg) : SeigeEngine(damage, sp), aoeDamage(aoedam), aoeTargets(aoetarg) {
}

//=========================================BATTERING RAM========================================

BatteringRam::BatteringRam(int id1, char* name, int h, int m, int a, int cost, int damage,
	int sp) : SeigeEngine(damage, sp) {
}

//==============================================TRANSPORT======================================================

Transport::Transport(int troop, float knots) :
	troopCapacity(troop), knotsSpeed(knots) {
}

//====================================================WARSHIP======================================

Warship::Warship(int id1, char* name, int h, int m, int a, int cost, int damage,
	int sp, int troop, float knots) : Assets(id1, name, h, m, a, cost), Transport(
		troop, knots), SeigeEngine(damage, sp) {
}


///////////////////////////////////////////////////////////PERSON///////////////////////////////////////////

//===============================================PERSON========================================

//default construcotr due to virutal inheritance issue
Person::Person() {
	age = 0;
	isAlive = false;
}

Person::Person(int id1, char* name, int a, bool alive) : Entity(id1, name),
age(a), isAlive(alive) {
}

int Person::getAge() {
	return age;
}

//================================================NOBLE=======================================

Noble::Noble(int id1, char* name, int a, bool alive, int realmid, float ambition,
	float diplomacy, float health, int father, int* childarr, int numchild) :
	Person(id1, name, a, alive), realmIdx(realmid), ambitionStat(ambition),
	diplomacyStat(diplomacy), healthMeter(0.0), fatherId(father),
	childCount(numchild)
{
	if (numchild > 0 && childarr != nullptr) {
		childrenIds = new int[numchild];

		for (int i = 0; i < numchild; i++) {
			childrenIds[i] = childarr[i];
		}
	} //if

	else {
		childrenIds = nullptr;
	}

} //constructor


int Noble::getAmbitionStat() {
	return ambitionStat;
}


Noble::~Noble() {
	delete[] childrenIds;
}

//=====================================================LANDED LORD=================================================

LandedLord::LandedLord(int id1, char* name, int a, bool alive, int realmid, float ambition,
	float diplomacy, float health, int father, int* childarr, int numchild, float strategy) : Noble(
		id1, name, a, alive, realmid, ambition, diplomacy, health, father,
		childarr, numchild), strategyStat(strategy) {
}

int LandedLord::getStrategyStat() {
	return strategyStat;
}

//=====================================================LANDLESS LORD=============================================

LandlessLord::LandlessLord(int id1, char* name, int a, bool alive, int realmid, float ambition,
	float diplomacy, float health, int father, int* childarr, int numchild, float plot) :
	Noble(id1, name, a, alive, realmid, ambition, diplomacy, health, father,
		childarr, numchild), plotProgree(plot) {
}

float LandlessLord::getPlotProgress() {
	return plotProgree;
}

void LandlessLord::setPlotProgress(float p) {
	this->plotProgree = p;
}

//=======================================================WARRIOR LORD================================

WarriorLord::WarriorLord(int id1, char* name, int a, bool alive, int realmid,
	float ambition, float diplomacy, float health, int father, int* childarr, int numchild,
	int h, int m, int a1, int cost, int attack, bool front, int shield) : Entity(id1, name),
	Noble(id1, name, a, alive, realmid, ambition, diplomacy, health,
		father, childarr, numchild), Knight(id1, name, h, m, a, cost,
			attack, front, shield) {
}

//======================================================ASSASSIN====================================================

Assassin::Assassin(int id1, char* name, int a, bool alive, float stealth, int clientid) :
	Person(id1, name, a, alive), stealthStat(stealth), clientIdx(clientid) {
}


//========================================================SAGE============================================================

Sage::Sage(int id1, char* name, int a, bool alive, float healing, float wis) :
	Person(id1, name, a, alive), healingPwr(healing), wisdom(wis) {
}




//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ KINGDOM $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$


Kingdom::Kingdom(const Kingdom& other) {    //copy contrucotr for rebellion adn army split

	//basically intialization for the rebel kingdom

	wealth = other.wealth;
	defenseStat = other.defenseStat;

	civilWar = 0.0f;

	currentLord = nullptr;
	rebelLord = nullptr;

	footSoldiers = nullptr;
	knights = nullptr;
	cavalry = nullptr;
	warriorLords = nullptr;

	footSoldierCount = 0;
	knightCount = 0;
	cavalryCount = 0;
	warriorLordCount = 0;

	courtiers = nullptr;
	courtierCount = 0;

	sages = nullptr;
	sageCount = 0;

	heirIds = nullptr;
	heirCount = 0;

	warships = nullptr;
	warshipCount = 0;

	batteringRam = nullptr;
	batteringRamCount = 0;

	catapult = nullptr;
	catapultCount = 0;
}


Kingdom::Kingdom() {

	currentLord = nullptr;
	rebelLord = nullptr;

	wealth = 0;
	defenseStat = 0;

	heirIds = nullptr;
	heirCount = 0;

	courtiers = nullptr;
	courtierCount = 0;

	footSoldiers = nullptr;
	footSoldierCount = 0;

	knights = nullptr;
	knightCount = 0;

	cavalry = nullptr;
	cavalryCount = 0;

	batteringRam = nullptr;
	batteringRamCount = 0;

	catapult = nullptr;
	catapultCount = 0;

	warships = nullptr;
	warshipCount = 0;

	warriorLords = nullptr;
	warriorLordCount = 0;

	sages = nullptr;
	sageCount = 0;

	civilWar = 0.0;
}


Cavalry** Kingdom::getCavalryArray() {
	return cavalry;
}

int Kingdom::getCavalryCount() {
	return cavalryCount;
}

void Kingdom::setCavalryCount(int cc) {
	this->cavalryCount = cc;
}

void Kingdom::addCavalry(Cavalry* c) {
	cavalry[cavalryCount++] = c;
}
void Kingdom::removeCavalry(int index) {
	cavalry[index] = nullptr;
}


Knight** Kingdom::getKnightArray() {
	return knights;
}

int Kingdom::getKnightCount() {
	return knightCount;
}

void Kingdom::setKnightCount(int kc) {
	this->knightCount = kc;
}

void Kingdom::addKnight(Knight* K) {
	knights[knightCount++] = K;
}

void Kingdom::removeKnight(int index) {
	knights[index] = nullptr;
}



WarriorLord** Kingdom::getWarriorLordArray() {
	return warriorLords;
}

int Kingdom::getWarriorLordCount() {
	return warriorLordCount;
}

void Kingdom::setWarriorLordCount(int wc) {
	this->warriorLordCount = wc;
}

void Kingdom::addWarrior(WarriorLord* w) {
	warriorLords[warriorLordCount++] = w;
}

void Kingdom::removeWarrior(int index) {
	warriorLords[index] = nullptr;
}



FootSoldier** Kingdom::getFootSoldierArray() {
	return footSoldiers;
}

int Kingdom::getFootSoldierCount() {
	return footSoldierCount;
}

void Kingdom::setFootSoldierCount(int fs) {
	this->footSoldierCount = fs;
}

void Kingdom::addFootSoldier(FootSoldier* fs) {
	footSoldiers[footSoldierCount++] = fs;
}

void Kingdom::removeFootSoldier(int index) {
	footSoldiers[index] = nullptr;
}

LandlessLord** Kingdom::getCourtierArray() {
	return courtiers;
}

int Kingdom::getCourtierCount() {
	return courtierCount;
}

void Kingdom::setCourtierCount(int c) {
	this->courtierCount = c;
}

int Kingdom::getWealth() {
	return wealth;
}

int Kingdom::getDefenseStat() {
	return defenseStat;
}

void Kingdom::setWealth(int w) {
	this->wealth = w;
}

void Kingdom::setDefenseStat(int ds) {
	this->defenseStat = ds;
}

void Kingdom::setCurrentLord(LandedLord* l) {
	this->currentLord = l;
}

LandedLord* Kingdom::getCurrentLord() {
	return currentLord;
}

void Kingdom::setRebelLord(LandlessLord* r) {
	this->rebelLord = r;
}

LandlessLord* Kingdom::getRebelLord() {
	return rebelLord;
}


LandlessLord* getRebelLord();

bool Kingdom::IsArmyDefeated() {

	for (int i = 0; i < cavalryCount; i++) {
		if (cavalry[i] != nullptr && cavalry[i]->getHp() > 0)
			return false;
	}

	for (int i = 0; i < knightCount; i++) {
		if (knights[i] != nullptr && knights[i]->getHp() > 0)
			return false;
	}

	for (int i = 0; i < footSoldierCount; i++) {
		if (footSoldiers[i] != nullptr && footSoldiers[i]->getHp() > 0)
			return false;
	}

	for (int i = 0; i < warriorLordCount; i++) {
		if (warriorLords[i] != nullptr && warriorLords[i]->getHp() > 0)
			return false;
	}
	return true;
}

void Kingdom::CivilWar() {
	//civil war system
	if (currentLord == 0) {
		civilWar += 30.0;
	}

	if (wealth < 0) {
		civilWar += 5.0;
	}

	for (int i = 0; i < courtierCount; i++) {
		if (courtiers[i] != nullptr) {
			int ambition = courtiers[i]->getAmbitionStat();

			if (ambition > 50) {
				civilWar += (ambition / 10.0);
			}
		}
	}

	if (currentLord != nullptr && currentLord->getStrategyStat() > 70) {
		civilWar -= 3.0;
	}

	int totalUnits = footSoldierCount + knightCount + cavalryCount + warriorLordCount;

	if (totalUnits > 200) {
		civilWar -= 1.0;
	}

	//bounding
	if (civilWar < 0) {
		civilWar = 0;
	}

	if (civilWar > 100) {
		civilWar = 100;
	}
}

float Kingdom::getCivilWar() {
	return civilWar;
}
void Kingdom::setCivilWar(float cw) {
	this->civilWar = cw;
}

void Kingdom::updatePlotProgress(int* relation, int kingCount) {
	for (int i = 0; i < courtierCount; i++) {

		LandlessLord* culprit = courtiers[i];
		if (culprit == nullptr) continue;

		int ambition = culprit->getAmbitionStat();

		if (ambition <= 60) continue;

		float progress = culprit->getPlotProgress();

		float dailyIncrease = (ambition - 60) / 5.0f;

		bool hasNegativeRelation = false;

		for (int j = 0; j < kingCount; j++) {
			if (relation[j] < 0) {
				hasNegativeRelation = true;
				break;
			}
		}

		if (hasNegativeRelation) {
			dailyIncrease += 2.0f;
		}

		if (currentLord != nullptr && currentLord->getStrategyStat() > 50) {
			dailyIncrease -= (currentLord->getStrategyStat() / 25.0f);
		}

		if (defenseStat > 50) {
			dailyIncrease -= 1.0f;
		}


		if (dailyIncrease < 0)
			dailyIncrease = 0;

		progress += dailyIncrease;
		culprit->setPlotProgress(progress);

		if (progress >= 100.0f) {
			solveAssassination(culprit);
			culprit->setPlotProgress(0);
		}
	}
}


void Kingdom::solveAssassination(LandlessLord* culprit) {
	if (currentLord == nullptr) return;

	float power = culprit->getAmbitionStat() + (wealth / 100.0f);

	float defense = currentLord->getStrategyStat() + (defenseStat / 10.0f);

	int age = currentLord->getAge();

	if (age < 40)
		defense += 5;
	else if (age > 40)
		defense -= 5;

	if (power > defense) {

		delete currentLord;
		currentLord = nullptr;


	}
	else {


		for (int i = 0; i < courtierCount; i++) {
			if (courtiers[i] == culprit) {
				delete courtiers[i];
				courtiers[i] = nullptr;
				break;
			}
		}

		// RELATION PENALTY (handled by Engine ideally)
		// You can leave comment or hook here

	}
}



Kingdom::~Kingdom() {

}

void Kingdom::performAction() {

	CivilWar();
	//updatePlotProgress();  called in engin esimulation function

}

////////////////////////////////////////////////////////FROST PEAKS KINGDOM////////////////////////////////////////////////

//=============================================frost peaks kingdom===================================

FrostPeaksKingdom::FrostPeaksKingdom() {
	coldAttritionModifier = 5;
}

FrostPeaksKingdom::FrostPeaksKingdom(int coldmod) {
	coldAttritionModifier = coldmod;
}

void FrostPeaksKingdom::applyColdWeather() {

	//decreasign hp by 5

	for (int i = 0; i < footSoldierCount; i++) {
		if (footSoldiers[i]) {
			footSoldiers[i]->setHp(footSoldiers[i]->getHp() - coldAttritionModifier);
		}
	}

	for (int i = 0; i < knightCount; i++) {
		if (knights[i]) {
			knights[i]->setHp(knights[i]->getHp() - coldAttritionModifier);
		}
	}

	for (int i = 0; i < cavalryCount; i++) {
		if (cavalry[i]) {
			cavalry[i]->setHp(cavalry[i]->getHp() - coldAttritionModifier);
		}
	}

	for (int i = 0; i < warriorLordCount; i++) {
		if (warriorLords[i]) {
			warriorLords[i]->setHp(warriorLords[i]->getHp() - coldAttritionModifier);
		}
	}
}

void FrostPeaksKingdom::performAction() {
	applyColdWeather();
	Kingdom::performAction();   //the overridden function

}

//============================================the peaks of frost==================================================

ThePeaksofFrost::ThePeaksofFrost(int coldmod) : FrostPeaksKingdom(coldmod) {

}


void ThePeaksofFrost::performAction() {

	FrostPeaksKingdom::performAction();
}

////////////////////////////////////////////////////THE VERDANT KINGDOM////////////////////////////////////////

//================================================the verdant kingdom===================================

TheVerdantKingdom::TheVerdantKingdom(int bonus) {
	foodSurplusBonus = bonus;
}

void TheVerdantKingdom::applyFoodSurplus() {
	wealth += foodSurplusBonus;
}

void TheVerdantKingdom::performAction() {
	applyFoodSurplus();
	Kingdom::performAction();

}

//===========================================the sky citadel============================================

TheSkyCitadel::TheSkyCitadel(int bonus) : TheVerdantKingdom(bonus) {
}

void TheSkyCitadel::performAction() {
	TheVerdantKingdom::performAction();

}

//========================================the verdant expanse==================================

TheVerdantExpanse::TheVerdantExpanse(int bonus) : TheVerdantKingdom(bonus) {

}

void TheVerdantExpanse::performAction() {
	TheVerdantKingdom::performAction();

}

//================================================the crimson sands========================================

TheCrimsonSands::TheCrimsonSands(int bonus) : TheVerdantKingdom(bonus) {

}

void TheCrimsonSands::performAction() {
	TheVerdantKingdom::performAction();

}

//================================================the azure river lands=======================================

TheAzureRiverLands::TheAzureRiverLands(int bonus) : TheVerdantKingdom(bonus) {

}

void TheAzureRiverLands::performAction() {
	TheVerdantKingdom::performAction();

}

////////////////////////////////////////////COASTAL KINGDOM//////////////////////////////////////////

//========================================coastal kingdom============================================

CoastalKingdom::CoastalKingdom(int bonus) : Kingdom() {
	navalTradeBonus = bonus;
}

void CoastalKingdom::applyTradeBonus() {
	int shipCount = 0;

	for (int i = 0; i < warshipCount; i++) {
		if (warships[i] != nullptr) shipCount++;
	}

	int baseTaxIncome = 100;   //currently defining it as defualt until something else is required

	int tradeBonus = (baseTaxIncome * navalTradeBonus * shipCount) / 100;

	wealth += tradeBonus;

}

void CoastalKingdom::performAction() {
	applyTradeBonus();
	Kingdom::performAction();

}

//================================================the sunland empire================================

TheSunLandEmpire::TheSunLandEmpire(int bonus) : CoastalKingdom(bonus) {

}

void TheSunLandEmpire::performAction() {
	CoastalKingdom::performAction();

}

//============================================the corsair isles================================

TheCorsairIsles::TheCorsairIsles(int bonus) : CoastalKingdom(bonus) {

}

void TheCorsairIsles::performAction() {
	CoastalKingdom::performAction();

}

//===========================================the tempest coast=====================================

TheTempestCoast::TheTempestCoast(int bonus) : CoastalKingdom(bonus) {

}

void TheTempestCoast::performAction() {
	CoastalKingdom::performAction();

}

//=========================================the imperial seat=========================

TheImperialSeat::TheImperialSeat(int bonus) : CoastalKingdom(bonus) {

}

void TheImperialSeat::performAction() {
	CoastalKingdom::performAction();

}

////////////////////////////////////////////////THE VOID RIFT////////////////////////////////////////////

TheVoidRift::TheVoidRift(int threatLvl) : Kingdom() {
	this->threatLvl = threatLvl;
}

void TheVoidRift::performAction() {
	threatLvl++;
}





//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ENGINE $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$


Engine::Engine(Kingdom** kingdom, int count, int** relations, int total) {
	this->kingdoms = kingdom;
	this->kingdomCount = count;
	this->relations = relations;
	this->totalDays = total;
	rebelFactions = nullptr;
	rebelCount = 0;
}


int** Engine::getRelations() {
	return relations;
}


void Engine::Simulation() {

	for (int day = 0; day < totalDays; day++) {

		for (int i = 0; i < kingdomCount; i++) {
			if (kingdoms[i] != nullptr) {
				kingdoms[i]->performAction();
			}
		}
	}

	for (int i = 0; i < kingdomCount; i++) {
		kingdoms[i]->updatePlotProgress(relations[i], kingdomCount);
	}

	checkForWar();
}

void Engine::checkForWar() {

	for (int i = 0; i < kingdomCount; i++) {
		for (int j = i + 1; j < kingdomCount; j++) {     //j=i+1 so that no duplicate kingdom is compared or sent
			if (relations[i][j] <= -80) {
				simulateFight(kingdoms[i], kingdoms[j]);
			}

		}
	}

}

void Engine::simulateFight(Kingdom* A, Kingdom* B) {
	while (!A->IsArmyDefeated() && !B->IsArmyDefeated()) {
		simulateHour(A, B);
	}
}

void Engine::simulateHour(Kingdom* A, Kingdom* B) {
	float damageA = calculateDamage(A);

	float damageB = calculateDamage(B);

	applyDamage(A, damageB);
	applyDamage(B, damageA);
}

float Engine::calculateDamage(Kingdom* A) {
	float totalDamage = 0;

	float bonusLethality = 1;   //augmentation by leading lord;

	LandedLord* lord = A->getCurrentLord();

	if (lord != nullptr) {
		bonusLethality += (lord->getStrategyStat()) / 100.0f;
	}

	Cavalry** currentcav = A->getCavalryArray();

	for (int i = 0; i < A->getCavalryCount(); i++) {
		if (currentcav[i] != nullptr && currentcav[i]->getHp() > 0) {
			totalDamage += currentcav[i]->getAttackPower() * bonusLethality;
		}
	}

	Knight** currentKnight = A->getKnightArray();

	for (int i = 0; i < A->getKnightCount(); i++) {
		if (currentKnight[i] != nullptr && currentKnight[i]->getHp() > 0) {
			totalDamage += currentKnight[i]->getAttackPower() * bonusLethality;
		}
	}

	WarriorLord** currentWarrior = A->getWarriorLordArray();

	for (int i = 0; i < A->getWarriorLordCount(); i++) {
		if (currentWarrior[i] != nullptr && currentWarrior[i]->getHp() > 0) {
			totalDamage += currentWarrior[i]->getAttackPower() * bonusLethality;
		}
	}

	FootSoldier** currentSoldier = A->getFootSoldierArray();

	for (int i = 0; i < A->getFootSoldierCount(); i++) {
		if (currentSoldier[i] != nullptr && currentSoldier[i]->getHp() > 0) {
			totalDamage += currentSoldier[i]->getAttackPower() * bonusLethality;
		}
	}

	return totalDamage;
}  //function


void Engine::applyDamage(Kingdom* A, float damage) {

	Cavalry** currentcav = A->getCavalryArray();

	for (int i = 0; i < A->getCavalryCount() && damage>0; i++) {
		if (currentcav[i] != nullptr && damage > 0) {
			float hp = currentcav[i]->getHp();

			if (damage >= hp) {
				damage -= hp;
				currentcav[i]->setHp(0);    //since daamge is greater than hp
			}

			else {
				float newhp = hp - damage;
				currentcav[i]->setHp(newhp);
				damage = 0;
			}
		}
	}  //for loop

	Knight** currentKnight = A->getKnightArray();

	for (int i = 0; i < A->getKnightCount() && damage>0; i++) {
		if (currentKnight[i] != nullptr && damage > 0) {
			float hp = currentKnight[i]->getHp();

			if (damage >= hp) {
				damage -= hp;
				currentKnight[i]->setHp(0);    //since daamge is greater than hp
			}

			else {
				float newhp = hp - damage;
				currentKnight[i]->setHp(newhp);
				damage = 0;
			}
		}
	}  //for loop


	WarriorLord** currentWarrior = A->getWarriorLordArray();

	for (int i = 0; i < A->getWarriorLordCount() && damage>0; i++) {
		if (currentWarrior[i] != nullptr && damage > 0) {
			int hp = currentWarrior[i]->getHp();

			if (damage >= hp) {
				damage -= hp;
				currentWarrior[i]->setHp(0);    //since daamge is greater than hp
			}

			else {
				int newhp = hp - damage;
				currentWarrior[i]->setHp(newhp);
				damage = 0;
			}
		}
	}  //for loop

	FootSoldier** currentSoldier = A->getFootSoldierArray();

	for (int i = 0; i < A->getFootSoldierCount() && damage>0; i++) {
		if (currentWarrior[i] != nullptr && damage > 0) {
			int hp = currentSoldier[i]->getHp();

			if (damage >= hp) {
				damage -= hp;
				currentSoldier[i]->setHp(0);    //since daamge is greater than hp
			}

			else {
				int newhp = hp - damage;
				currentSoldier[i]->setHp(newhp);
				damage = 0;
			}
		}
	}  //for loop

} //fun


void Engine::triggerWar() {

	for (int i = 0; i < kingdomCount; i++) {
		Kingdom* K = kingdoms[i];

		if (K->getCivilWar() >= 100.0) {

			LandlessLord* rebelLeader = nullptr;
			LandlessLord** courtiers = K->getCourtierArray();

			for (int j = 0; j < K->getCourtierCount(); j++) {

				LandlessLord* L = courtiers[j];

				if (L == nullptr) continue;

				if (rebelLeader == nullptr || L->getAmbitionStat()
			> rebelLeader->getAmbitionStat()) {
					rebelLeader = L;
				}
			}

			//K->setCurrentLord(rebelLeader);


			Kingdom* rebel = new Kingdom(*K);  //copy contrucotr

			rebel->setWealth(K->getWealth() / 2);
			rebel->setDefenseStat(K->getDefenseStat() / 2);

			K->setWealth(K->getWealth() / 2);
			K->setDefenseStat(K->getDefenseStat() / 2);

			K->setRebelLord(rebelLeader);
			K->setCurrentLord(nullptr);                        //to set the rebel lord as the current leader

			ArmySplit(K, rebel);

			K->setCivilWar(0.0);

			Kingdom** newRebels = new Kingdom * [rebelCount + 1];

			for (int r = 0; r < rebelCount; r++) {
				newRebels[r] = rebelFactions[r];
			}

			newRebels[rebelCount] = rebel;

			delete[] rebelFactions;
			rebelFactions = newRebels;

			rebelCount++;



		} //if
	} //for
}//fun

void Engine::ArmySplit(Kingdom* parent, Kingdom* rebel) {

	int start1 = parent->getFootSoldierCount() / 2;
	FootSoldier** fsParent = parent->getFootSoldierArray();

	for (int i = parent->getFootSoldierCount() - 1; i >= start1; i--) {

		rebel->addFootSoldier(fsParent[i]);
		parent->removeFootSoldier(i);
	}


	int start2 = parent->getKnightCount() / 2;
	Knight** kParent = parent->getKnightArray();

	for (int i = parent->getKnightCount() - 1; i >= start2; i--) {

		rebel->addKnight(kParent[i]);
		parent->removeKnight(i);
	}


	int start3 = parent->getWarriorLordCount() / 2;
	WarriorLord** wParent = parent->getWarriorLordArray();

	for (int i = parent->getWarriorLordCount() - 1; i >= start3; i--) {

		rebel->addWarrior(wParent[i]);
		parent->removeWarrior(i);
	}


	int start4 = parent->getCavalryCount() / 2;
	Cavalry** cParent = parent->getCavalryArray();

	for (int i = parent->getCavalryCount() - 1; i >= start4; i--) {

		rebel->addCavalry(cParent[i]);
		parent->removeCavalry(i);
	}

}

