#include <iostream>
#include <ctime>
#include <fstream>
#include "DbConnection/DbConnection.h"
#include "Config/ConfigFile.h"
#include "TripChain/TripChain.h"

using namespace std;

//#define DB_HOST "tcp://192.168.0.60:3306"
//#define DB_USER "sunkus711"
//#define DB_PASS "rmlduqrml1"
//#define DB_NAME "tripchain_daejeon_test"

void makeTM(tm* date_tm, string date, time_t* date_t);
void timeTotm_str(tm* date_tm, time_t* date_t, string* str);

//unordered_multimap<int, RAWDATA*> data;
//  ./SmartCard_RawData YYYYMMDD YYYYMMDD
int main(int argc, char* argv[]) // 1번째 인자 : 가장 작은 날짜, 없을 시 테이블 내에 가장 작은 날짜 검색
{
	ConfigFile config("conf/dbinfo.conf");
	string DB_HOST, DB_USER, DB_PASS, DB_NAME;
	DB_HOST = config.read<string>("DB_HOST");
	DB_USER = config.read<string>("DB_USER");
	DB_PASS = config.read<string>("DB_PASS");
	DB_NAME = config.read<string>("DB_NAME");

	// db연결
	DBCONNECTION db(DB_HOST, DB_USER, DB_PASS, DB_NAME);

	// 최소 날짜 찾기
	cout << "최소날짜와 최대날짜 찾는 중...";
	string mindate, maxdate, cut;
	tm* mindate_tm = new tm,* maxdate_tm = new tm,* cut_tm = new tm;
	time_t mindate_t, maxdate_t, cut_t;

	if(argc < 2) // 명령줄인자가 없으면 테이블내에서 최소값 검색
	{
		db.runQuery("select min(승차일시) as 최소값 from CARD_DATA_NORM");
		db.getRes()->next();
		mindate = db.getRes()->getString("최소값").substr(0, 8) + "040000";
	}
	else
	{
		mindate = argv[1];
		mindate += "040000";
	}

	// 최대 날짜 찾기
	db.runQuery("select max(승차일시) as 최대값 from CARD_DATA_NORM");
	db.getRes()->next();
	maxdate = db.getRes()->getString("최대값").substr(0, 8) + "040000";
	makeTM(mindate_tm, mindate, &mindate_t);
	makeTM(maxdate_tm, maxdate, &maxdate_t);
	if(difftime(maxdate_t, mindate_t) == 0) // 최소날짜와 최대날짜가 같다면
	{
		maxdate_t = mindate_t + 24*60*60; // 최소날짜의 다음날 4시까지
		timeTotm_str(maxdate_tm, &maxdate_t, &maxdate); // 바꾼 날짜 적용
	}
	cout << "완료!" << endl << "최소날짜 : " << mindate << ", 최대날짜 : " << maxdate << endl;

	// db에서 하루치 정보를 뽑아서 돌림
	for(cut_t = mindate_t; cut_t <= maxdate_t; cut_t += 24*60*60)
	{
		cout << "날짜 : " << cut_t << endl;
		time_t next_t = cut_t + 24*60*60;
		tm* next_tm = new tm;
		string next;
		timeTotm_str(cut_tm, &cut_t, &cut);
		timeTotm_str(next_tm, &next_t, &next);

		// 원시데이터 가져오기
		TRIPCHAIN* tripchain;
		db.runQuery("select * from CARD_DATA_NORM where 승차일시 between '" + cut + "' and '" + next + "'");
		if(db.getRes() != NULL)
		{
			tripchain = new TRIPCHAIN(db.getRes(), *cut_tm); // TRIPCHAIN 클래스 생성
		}
		else
		{
			cout << "원시데이터 없음" << endl;
			return 1;
		}
		cout << "완료!";

		cout << "트립체인데이터 생성 중...";
		// csv파일 생성
		ofstream o("tripchain" + cut + ".csv");
		tripchain->print(&o);
		o.close();
		cout << "완료!" << endl << "DB에 데이터 입력 중...";

		// 테이블에 csv파일 입력
		db.runQuery("LOAD DATA LOCAL INFILE 'tripchain" + cut + ".csv' INTO TABLE CARD_DATA_TRIPCHAIN FIELDS TERMINATED BY ',' ENCLOSED BY '\"' LINES TERMINATED BY '\\n' IGNORE 1 LINES");
		cout << "완료!" << endl;

		delete next_tm;
		delete tripchain;
	}

	return 0;
}

void makeTM(tm* date_tm, string date, time_t* date_t)
{
	date_tm->tm_year = atoi(date.substr(0,4).c_str()) - 1900;
	date_tm->tm_mon = atoi(date.substr(4,2).c_str()) - 1;
	date_tm->tm_mday = atoi(date.substr(6,2).c_str());
	date_tm->tm_hour = atoi(date.substr(8,2).c_str());
	date_tm->tm_min = atoi(date.substr(10,2).c_str());
	date_tm->tm_sec = atoi(date.substr(12,2).c_str());
	*date_t = mktime(date_tm);
}

void timeTotm_str(tm* date_tm, time_t* date_t, string* str)
{
	localtime_r(date_t, date_tm); // 바꾼 날짜 적용
	*str = TRIPCHAINDATA::dateTostr(*date_tm);
}
