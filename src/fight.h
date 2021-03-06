// Part of Bylins http://www.mud.ru

#ifndef _FIGHT_H_
#define _FIGHT_H_

#include "conf.h"
#include "sysdep.h"
#include "structs.h"
#include "utils.h"
#include "char.hpp"

namespace FightSystem
{

enum DmgType { UNDEF_DMG, PHYS_DMG, MAGE_DMG };

// attack_hit_text[]
enum
{
	type_hit, type_skin, type_whip, type_slash, type_bite,
	type_bludgeon, type_crush, type_pound, type_claw, type_maul,
	type_thrash, type_pierce, type_blast, type_punch, type_stab,
	type_pick, type_sting
};

enum
{
	// ����� �����
	IGNORE_SANCT,
	// ����� ������
	IGNORE_PRISM,
	// ����� �����
	IGNORE_ARMOR,
	// �������������� ����������� �����
	HALF_IGNORE_ARMOR,
	// ����� ����������
	IGNORE_ABSORBE,
	// ������ �������
	NO_FLEE,
	// ���� ����
	CRIT_HIT,
	// ����� �������� ������ �� ��������� ����
	IGNORE_FSHIELD,
	// ����� ���� �� ����������� ������� ��� ��������� �������
	MAGIC_REFLECT,
	// ������ ����� �������� ���
	VICTIM_FIRE_SHIELD,
	// ������ ����� ��������� ���
	VICTIM_AIR_SHIELD,
	// ������ ����� ������� ���
	VICTIM_ICE_SHIELD,
	// ���� ��������� �� ��������� ���� � ������� ������
	DRAW_BRIEF_FIRE_SHIELD,
	// ���� ��������� �� ���������� ���� � ������� ������
	DRAW_BRIEF_AIR_SHIELD,
	// ���� ��������� �� �������� ���� � ������� ������
	DRAW_BRIEF_ICE_SHIELD,
	// ���� ������� �� ���. �������
	DRAW_BRIEF_MAG_MIRROR,

	// ���-�� ������
	HIT_TYPE_FLAGS_NUM
};

} // namespace FightSystem

/**
 * ��� ����� �� ����� ��� ����� ��������� �����:
 * Damage obj(SkillDmg(SKILL_NUM), dam, FightSystem::UNDEF_DMG|PHYS_DMG|MAGE_DMG)
 * obj.process(ch, victim);
 */
struct SkillDmg
{
	SkillDmg(int num) : skill_num(num) {};
	int skill_num;
};

/**
 * ��� ����� � ����� ��� ����� ��������� �����:
 * Damage obj(SpellDmg(SPELL_NUM), dam, FightSystem::UNDEF_DMG|PHYS_DMG|MAGE_DMG)
 * obj.process(ch, victim);
 */
struct SpellDmg
{
	SpellDmg(int num) : spell_num(num) {};
	int spell_num;
};

/**
 * ��� ����� � ���������� ������ ��� ����� ��������� ����� (������ ����� ����� messages):
 * Damage obj(SimpleDmg(TYPE_NUM), dam, FightSystem::UNDEF_DMG|PHYS_DMG|MAGE_DMG)
 * obj.process(ch, victim);
 */
struct SimpleDmg
{
	SimpleDmg(int num) : msg_num(num) {};
	int msg_num;
};

/**
 * ������� �������������, ������������ ����:
 *   dam - �������� �����
 *   msg_num - �������� ��� ����� skill_num/spell_num/hit_type
 *   dmg_type - UNDEF_DMG/PHYS_DMG/MAGE_DMG
 * ��������� �� �������������:
 *   ch_start_pos - ���� ����� ������������ ����� �� ������� ���������� (��� �����)
 *   victim_start_pos - ���� ����� ������������ ����� �� ������� ������ (���/��� �����)
 */
class Damage
{
public:
	// ��������� ������ �������� �������
	Damage() { zero_init(); };
	// �����
	Damage(SkillDmg obj, int in_dam, FightSystem::DmgType in_dmg_type)
	{
		zero_init();
		skill_num = obj.skill_num;
		dam = in_dam;
		dmg_type = in_dmg_type;
	};
	// ����������
	Damage(SpellDmg obj, int in_dam, FightSystem::DmgType in_dmg_type)
	{
		zero_init();
		spell_num = obj.spell_num;
		dam = in_dam;
		dmg_type = in_dmg_type;
	};
	// ������ �����
	Damage(SimpleDmg obj, int in_dam, FightSystem::DmgType in_dmg_type)
	{
		zero_init();
		msg_num = obj.msg_num;
		dam = in_dam;
		dmg_type = in_dmg_type;
	};

	int process(CHAR_DATA *ch, CHAR_DATA *victim);

	// ����� ����������
	int dam;
	// flags[CRIT_HIT] = true, dam_critic = 0 - ����������� ����
	// flags[CRIT_HIT] = true, dam_critic > 0 - ���� ������ ������
	int dam_critic;
	// ��� ����� (���/���/�������)
	int dmg_type;
	// ��. �������� � HitData
	int skill_num;
	// ����� ����������, ���� >= 0
	int spell_num;
	// ��. �������� � HitData, �� ����� ����� ���� -1
	int hit_type;
	// ����� ��������� �� ����� �� ����� messages
	// ������ ������ ������� � ������ process
	int msg_num;
	// ����� ������ �� HitType
	std::bitset<FightSystem::HIT_TYPE_FLAGS_NUM> flags;
	// ������� ���������� �� ������ ����� (�� ������� ����� = �������� ���������)
	int ch_start_pos;
	// ������� ������ �� ������ ����� (�� ������� ����� = �������� ���������)
	int victim_start_pos;

private:
	// ���� ���� ����� ���������� ���������� ��� �������������
	void zero_init();
	// ���� msg_num, ch_start_pos, victim_start_pos
	// ��������� � ������ process, ����� ��� ��� ���������
	void post_init(CHAR_DATA *ch, CHAR_DATA *victim);
	void post_init_shields(CHAR_DATA *victim);
	// process()
	bool magic_shields_dam(CHAR_DATA *ch, CHAR_DATA *victim);
	void armor_dam_reduce(CHAR_DATA *ch, CHAR_DATA *victim);
	bool dam_absorb(CHAR_DATA *ch, CHAR_DATA *victim);
	void process_death(CHAR_DATA *ch, CHAR_DATA *victim);
	void send_critical_message(CHAR_DATA *ch, CHAR_DATA *victim);
	void dam_message(CHAR_DATA* ch, CHAR_DATA* victim) const;

	// �������� ����� �� ��������� ����
	int fs_damage;
	// ������ ��� �������� ������ �����, ������������ ����� ������ � �������
	// ���� �� flags ���� ��������������� �����
	std::string brief_shields_;
};

// fight.cpp

void set_fighting(CHAR_DATA *ch, CHAR_DATA *victim);
void stop_fighting(CHAR_DATA *ch, int switch_others);
void perform_violence();

// fight_hit.cpp

int compute_armor_class(CHAR_DATA *ch);
bool check_mighthit_weapon(CHAR_DATA *ch);
void apply_weapon_bonus(int ch_class, int skill, int *damroll, int *hitroll);

void hit(CHAR_DATA *ch, CHAR_DATA *victim, int type, int weapon);

// fight_stuff.cpp

void die(CHAR_DATA *ch, CHAR_DATA *killer);
void raw_kill(CHAR_DATA *ch, CHAR_DATA *killer);

void alterate_object(OBJ_DATA *obj, int dam, int chance);
void alt_equip(CHAR_DATA *ch, int pos, int dam, int chance);

void char_dam_message(int dam, CHAR_DATA *ch, CHAR_DATA *victim, bool mayflee);
void test_self_hitroll(CHAR_DATA *ch);

#endif
