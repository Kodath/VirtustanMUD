// $RCSfile$     $Date$     $Revision$
// Copyright (c) 2008 Krodo
// Part of Bylins http://www.mud.ru

#ifndef CHAR_PLAYER_HPP_INCLUDED
#define CHAR_PLAYER_HPP_INCLUDED

#include "conf.h"
#include <string>
#include <array>
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>
#include <boost/cstdint.hpp>

#include "sysdep.h"
#include "structs.h"
#include "quested.hpp"
#include "mobmax.hpp"
#include "remember.hpp"
#include "char.hpp"
#include "dps.hpp"
#include "morph.hpp"
#include "map.hpp"
#include "reset_stats.hpp"
#include "boards.h"

// ���-�� ����������� ��������� ������ � �����
const int START_STATS_TOTAL = 6;

// ��� ����������� ������
enum
{
	DIS_OFFTOP_MESSAGE,
	DIS_EXCHANGE_MESSAGE,
	DIS_TOTAL_NUM
};

class Player : public Character
{
public:
	Player();
	int get_pfilepos() const;
	void set_pfilepos(int pfilepos);

	room_rnum get_was_in_room() const;
	void set_was_in_room(room_rnum was_in_room);

	std::string const & get_passwd() const;
	void set_passwd(std::string const & passwd);

	room_rnum get_from_room() const;
	void set_from_room(room_rnum was_in_room);

	void remort();
	void reset();

	int get_start_stat(int num);
	void set_start_stat(int stat_num, int number);

	void set_last_tell(const char *text);
	std::string const & get_last_tell();

	void set_answer_id(int id);
	int get_answer_id() const;

	// ������� �� CharRemember
	void remember_add(std::string text, int flag);
	std::string remember_get(int flag) const;
	bool remember_set_num(unsigned int num);
	unsigned int remember_get_num() const;

	// ������� �� Quested
	void quested_add(CHAR_DATA *ch, int vnum, char *text);
	bool quested_remove(int vnum);
	bool quested_get(int vnum) const;
	std::string quested_get_text(int vnum) const;
	std::string quested_print() const;
	void quested_save(FILE *saved) const; // TODO �� ������

	// ������� �� MobMax
	int mobmax_get(int vnum) const;
	void mobmax_add(CHAR_DATA *ch, int vnum, int count, int level);
	void mobmax_load(CHAR_DATA *ch, int vnum, int count, int level);
	void mobmax_remove(int vnum);
	void mobmax_save(FILE *saved) const; // TODO �� ������

	// ������� �� Dps
	void dps_add_dmg(int type, int dmg, int over_dmg, CHAR_DATA *ch = 0);
	void dps_clear(int type);
	void dps_print_stats(CHAR_DATA *coder = 0);
	void dps_print_group_stats(CHAR_DATA *ch, CHAR_DATA *coder = 0);
	void dps_set(DpsSystem::Dps *dps);
	void dps_copy(CHAR_DATA *ch);
	void dps_end_round(int type, CHAR_DATA *ch = 0);
	void dps_add_exp(int exp, bool battle = false);

	void save_char();
	int load_char_ascii(const char *name, bool reboot = 0);

	bool get_disposable_flag(int num);
	void set_disposable_flag(int num);

	// ���� ������ ��� ������� �� motion_, �� ����� ���� ��������
	bool is_active() const;
	void set_motion(bool flag);

	void map_olc();
	void map_olc_save();
	bool map_check_option(int num) const;
	void map_set_option(unsigned num);
	void map_print_to_snooper(CHAR_DATA *imm);
	void map_text_olc(const char *arg);
	const MapSystem::Options * get_map_options() const;

	int get_ext_money(unsigned type) const;
	void set_ext_money(unsigned type, int num, bool write_log = true);
	int get_today_torc();
	void add_today_torc(int num);

	int get_reset_stats_cnt(ResetStats::Type type) const;
	void inc_reset_stats_cnt(ResetStats::Type type);

	time_t get_board_date(Boards::BoardTypes type) const;
	void set_board_date(Boards::BoardTypes type, time_t date);

private:
	// ���������� ����� � ����� �����-����� (�� ����� �����, �� ������ ������ ������ �� ���)
	// TODO: ������ ��� ����� ���������� ������ ����������� ������ �� ����� ��� ������ ���� � �.�. �����, ����������
	// get_ptable_by_name ��� find_name (������������ ���� ������) � ������ ������ �� ��/���, ���� ��� ����� ���-����
	int pfilepos_;
	// �������, � ������� ��� ��� �� ����, ��� ��� ��������� � �������� (linkdrop)
	room_rnum was_in_room_;
	// ��� ������
	std::string passwd_;
	// �������, ��� ��� ��� �� ������ char_from_room (was_in_room_ ��� ��� ������������ �� �� ������)
	// � ������ ������ ���� ����� ��� �������� ���� �� �� ��� ����� �� ����� ����� ��������, �� ����� � ��� ����� �����������
	room_rnum from_room_;
	// ��������� �����
	boost::array<int, START_STATS_TOTAL> start_stats_;
	// ���������
	CharRemember remember_;
	// ����� ����������� ������� � ������������ �������
	Quested quested_;
	// ������� �� ��������� �����
	MobMax mobmax_;
	// ��������� ��������� ������ (�� �����)
	std::string last_tell_;
	// id ���������� ��������� (��� ������)
	long answer_id_;
	// '�����' ������������ � ������, ���� ��� �����
	DpsSystem::Dps dps_;
	// ����������� �����
	std::bitset<DIS_TOTAL_NUM> disposable_flags_;
	// false, ���� ��� ������� ��� ���������� ����� check_idling � ���� �� �������� � �����
	bool motion_;
	// ����� ��������� ������ �����
	MapSystem::Options map_options_;
	// ���. ������ (������)
	boost::array<int, ExtMoney::TOTAL_TYPES> ext_money_;
	// ������� �����, � ��������� �� ������, ������� ��� �������
	std::pair<boost::uint8_t /* day 1-31 */, int> today_torc_;
	// ���-�� ������� ������������� ����� ����
	std::array<int, ResetStats::Type::TOTAL_NUM> reset_stats_cnt_;
	// ��������� ������� � ����������� ���������� �� ������
	std::array<time_t, Boards::TYPES_NUM> board_date_;
};

namespace PlayerSystem
{

int con_natural_hp(CHAR_DATA *ch);
int con_add_hp(CHAR_DATA *ch);
int con_total_hp(CHAR_DATA *ch);
unsigned weight_dex_penalty(CHAR_DATA* ch);

} // namespace PlayerSystem

#endif // CHAR_PLAYER_HPP_INCLUDED
