/*
 * TRIPCHAINDATA.cpp
 *
 *  Created on: 2018. 3. 19.
 *      Author: lsh
 */

#include "TripChainData.h"

TRIPCHAINDATA::TRIPCHAINDATA() {
	// TODO Auto-generated constructor stub
	TOTAL_USER_CNT = 0;				// 총 이용객수
	TOTAL_AMT = 0;					// 총 이용금액
	TOTAL_DIST = 0;					// 총통행거리
	TOTAL_BRDNG_TIME = 0;			// 총탑승시간
	TOTAL_TIME = 0;
}

TRIPCHAINDATA::TRIPCHAINDATA(sql::ResultSet* res)
{
	setDate(&TC_DT, res->getString("승차일시")); // 날짜
	VIRTL_CARD_NO = res->getString("가상카드번호"); // 카드번호
	TRNSC_ID = res->getString("트랜젝션ID"); // 트랜젝션ID
	EXCCLC_AREA_CD = res->getString("정산지역코드");			// 정산지역코드
	TRCRD_USER_GB = res->getString("사용자구분코드"); // 교통카드사용자구분코드
	setDate(&TKCAR_DT_FIRST, res->getString("승차일시")); // 최초승차일시
	TKCAR_STTN_ID_FIRST = res->getString("승차정류장ID"); // 최초승차역ID
	ROUTE_ID_FIRST = res->getString("노선ID"); // 최초 승차버스노선ID
	TOTAL_USER_CNT = 0;				// 총 이용객수
	TOTAL_AMT = 0;					// 총 이용금액
	TOTAL_DIST = 0;					// 총통행거리
	TOTAL_BRDNG_TIME = 0;			// 총탑승시간
	TOTAL_TIME = 0;
	addData(res, 0);
}

TRIPCHAINDATA::~TRIPCHAINDATA() {
	// TODO Auto-generated destructor stub
}

void TRIPCHAINDATA::addData(sql::ResultSet* res, int trnsit_cnt)
{
	TFCMN_CD[trnsit_cnt] = res->getString("교통수단코드");
	ROUTE_ID[trnsit_cnt] = res->getString("노선ID");
	CAR_ID[trnsit_cnt] = res->getString("차량ID");
	PASNG_DIST[trnsit_cnt] = res->getString("이용거리");
	TKCAR_DT[trnsit_cnt] = res->getString("승차일시");
	GFF_DT[trnsit_cnt] = res->getString("하차일시");
	TKCAR_STTN_ID[trnsit_cnt] = res->getString("승차정류장ID");
	GFF_STTN_ID[trnsit_cnt] = res->getString("하차정류장ID");
	TFCBIZER_ID[trnsit_cnt] = res->getString("교통사업자ID");

	setDate(&GFF_DT_LAST, res->getString("하차일시")); // 최종하차일시
	GFF_STTN_ID_LAST = res->getString("하차정류장ID"); // 최종하차역ID
	ROUTE_ID_LAST = res->getString("노선ID"); // 최종 승차버스노선ID
	// 총 이용객
	int user_cnt = stoi(res->getString("이용객수"));
	if(user_cnt > TOTAL_USER_CNT) TOTAL_USER_CNT = user_cnt;

	TOTAL_AMT += stoi(res->getString("승차금액")) + stoi(res->getString("하차금액"));
	TOTAL_DIST += stoi(PASNG_DIST[trnsit_cnt]);

	string brdng_time = res->getString("탑승시간");
	if(brdng_time != "~")
	{
		TOTAL_BRDNG_TIME += stoi(res->getString("탑승시간"));
	}
	else
	{
		TOTAL_BRDNG_TIME = 0;
	}
	if(GFF_DT_LAST.tm_year != 0)
	{
		TOTAL_TIME = difftime(mktime(&GFF_DT_LAST), mktime(&TKCAR_DT_FIRST));
	}
	else
	{
		TOTAL_TIME = 0;
	}
	TRNSIT_CNTC = to_string(trnsit_cnt);
}

void TRIPCHAINDATA::setDate(tm* tm, string date)
{
	if(date != "~")
	{
		tm->tm_year = atoi(date.substr(0,4).c_str()) - 1900;
		tm->tm_mon = atoi(date.substr(4,2).c_str()) - 1;
		tm->tm_mday = atoi(date.substr(6,2).c_str());
		tm->tm_hour = atoi(date.substr(8,2).c_str());
		tm->tm_min = atoi(date.substr(10,2).c_str());
		tm->tm_sec = atoi(date.substr(12,2).c_str());
		mktime(tm);

	}
	else
	{
		tm->tm_year = 0;
	}
}

string TRIPCHAINDATA::dateTostr(tm tm)
{
	string output;
	if(tm.tm_year > 0)
	{
		output = to_string(tm.tm_year + 1900);
		if(tm.tm_mon < 9)
		{
			output += "0";
		}
		output += to_string(tm.tm_mon + 1);
		if(tm.tm_mday < 10)
		{
			output += "0";
		}
		output += to_string(tm.tm_mday);
		if(tm.tm_hour < 10)
		{
			output += "0";
		}
		output += to_string(tm.tm_hour);
		if(tm.tm_min < 10)
		{
			output += "0";
		}
		output += to_string(tm.tm_min);
		if(tm.tm_sec < 10)
		{
			output += "0";
		}
		output += to_string(tm.tm_sec);
	}
	else
	{
		output = "~";
	}
	return output;
}

void TRIPCHAINDATA::print(ostream* fout)
{
	*fout <<
			"\"" << dateTostr(TC_DT).substr(0,8) << "\"," <<
			"\"" << VIRTL_CARD_NO << "\"," <<
			"\"" << TRNSC_ID << "\"," <<
			"\"" << EXCCLC_AREA_CD << "\"," <<
			"\"" << TRNSIT_CNTC << "\"," <<
			"\"" << TRCRD_USER_GB << "\",";
	for(int i = 0; i < 5; i++)
	{
		*fout <<
				"\"" << TFCMN_CD[i] << "\"," <<
				"\"" << ROUTE_ID[i] << "\"," <<
				"\"" << CAR_ID[i] << "\"," <<
				"\"" << PASNG_DIST[i] << "\"," <<
				"\"" << TKCAR_DT[i] << "\"," <<
				"\"" << GFF_DT[i] << "\"," <<
				"\"" << TKCAR_STTN_ID[i] << "\"," <<
				"\"" << GFF_STTN_ID[i] << "\"," <<
				"\"" << TFCBIZER_ID[i] << "\",";
	}
	*fout <<
			"\"" << dateTostr(TKCAR_DT_FIRST) << "\"," <<
			"\"" << TKCAR_STTN_ID_FIRST << "\"," <<
			"\"" << ROUTE_ID_FIRST << "\"," <<
			"\"" << dateTostr(GFF_DT_LAST) << "\"," <<
			"\"" << GFF_STTN_ID_LAST << "\"," <<
			"\"" << ROUTE_ID_LAST << "\"," <<
			"\"" << TOTAL_USER_CNT << "\"," <<
			"\"" << TOTAL_AMT << "\"," <<
			"\"" << TOTAL_DIST << "\",";
	if(TOTAL_BRDNG_TIME != 0)
	{
		*fout << "\"" << TOTAL_BRDNG_TIME << "\",";
	}
	else
	{
		*fout << "\"~\",";
	}
	if(TOTAL_TIME != 0)
	{
		*fout << "\"" << TOTAL_TIME << "\",";
	}
	else
	{
		*fout << "\"~\",";
	}
	string wday;
	switch(TC_DT.tm_wday)
	{
	case 0:
		wday = "일";
		break;
	case 1:
		wday = "월";
		break;
	case 2:
		wday = "화";
		break;
	case 3:
		wday = "수";
		break;
	case 4:
		wday = "목";
		break;
	case 5:
		wday = "금";
		break;
	case 6:
		wday = "토";
		break;
	case 7:
		wday = "일";
		break;
	}
	*fout <<	"\"" << wday << "\"" << endl;
}
