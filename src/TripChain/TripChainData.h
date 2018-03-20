/*
 * TRIPCHAINDATA.h
 *
 *  Created on: 2018. 3. 19.
 *      Author: lsh
 */

#ifndef SRC_TRIPCHAIN_TRIPCHAINDATA_H_
#define SRC_TRIPCHAIN_TRIPCHAINDATA_H_
#include <string>
#include <ctime>
#include "../DbConnection/DbConnection.h"
using namespace std;

class TRIPCHAINDATA {
public:
	TRIPCHAINDATA();
	TRIPCHAINDATA(sql::ResultSet* res);
	virtual ~TRIPCHAINDATA();

	// function
	void addData(sql::ResultSet* res, int trnsit_cnt); // 환승횟수에 알맞는 칼럼에 데이터 입력
	static void setDate(tm* tm, string date);
	void print(ostream* fout); // 출력 함수
	static string dateTostr(tm tm);

	tm TC_DT;							// 날짜
	string VIRTL_CARD_NO;			// 카드번호
	string TRNSC_ID;				// 트랜젝션ID
	string EXCCLC_AREA_CD;			// 정산지역코드
	string TRNSIT_CNTC;				// 환승횟수
	string TRCRD_USER_GB;			// 교통카드사용자구분
	string TFCMN_CD[5];				// 교통수단코드 1~5
	string ROUTE_ID[5];				// 노선ID 1~5
	string CAR_ID[5];				// 차량ID 1~5
	string PASNG_DIST[5];			// 통행거리 1~5
	string TKCAR_DT[5];					// 승차일시 1~5
	string GFF_DT[5];					// 하차일시 1~5
	string TKCAR_STTN_ID[5];		// 승차역ID 1~5
	string GFF_STTN_ID[5];			// 하차역ID 1~5
	string TFCBIZER_ID[5];			// 교통사업자ID 1~5
	tm TKCAR_DT_FIRST;				// 최초승차일시
	string TKCAR_STTN_ID_FIRST;	// 최초승차역ID
	string ROUTE_ID_FIRST;			// 최초 승차버스노선ID
	tm GFF_DT_LAST;					// 최종하차일시
	string GFF_STTN_ID_LAST;		// 최종하차역ID
	string ROUTE_ID_LAST;			// 최종하차버스노선ID
	int TOTAL_USER_CNT;				// 총 이용객수
	int TOTAL_AMT;					// 총 이용금액
	int TOTAL_DIST;					// 총통행거리
	int TOTAL_BRDNG_TIME;			// 총탑승시간
	int TOTAL_TIME;					// 총소요시간
};

#endif /* SRC_TRIPCHAIN_TRIPCHAINDATA_H_ */
