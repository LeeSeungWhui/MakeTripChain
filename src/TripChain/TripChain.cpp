/*
 * TRIPCHAIN.cpp
 *
 *  Created on: 2018. 3. 19.
 *      Author: lsh
 */

#include "TripChain.h"

TRIPCHAIN::TRIPCHAIN() {
	// TODO Auto-generated constructor stub

}

TRIPCHAIN::TRIPCHAIN(sql::ResultSet* res)
{
	int trnsit_cnt;
	string card_no_pre;
	string card_no_next;
	while(res->next() == true)
	{
		trnsit_cnt = stoi(res->getString("환승횟수")); // 현재 row의 환승횟수

		if(trnsit_cnt == 0) // 환승 횟수가 0인경우는 새로운 row 생성
		{
			this->data.push_back(new TRIPCHAINDATA(res));
			card_no_pre = res->getString("가상카드번호"); // 환승이 0인 경우의 카드번호
		}
		else if(trnsit_cnt > 3) // 환승횟수가 4 이상인 경우는 잘못된 데이터
		{

		}
		else // 환승 횟수가 1~3 일때는 기존 트립체인에 채워넣음
		{
			card_no_next = res->getString("가상카드번호"); // 현재 카드번호
			if(card_no_pre == card_no_next) // 카드번호, 승차시간 순으로 정렬되어있으므로 환승했다면 이전 카드번호와 같음
			{
				this->data.back()->addData(res,trnsit_cnt); // 환승횟수에 알맞은 칼럼에 데이터 입력
			}
			else // 예외처리 필요?
			{

			}
		}
	}
}

TRIPCHAIN::~TRIPCHAIN() {
	// TODO Auto-generated destructor stub
}

void TRIPCHAIN::print(ostream* fout)	// 출력 함수
{
	*fout <<
				"날짜,카드번호,트랜젝션ID,정산지역코드,환승횟수,교통카드사용자구분," <<
				"교통수단코드1,버스노선ID1,차량ID1,통행거리1,승차일시1,하차일시1,승차역ID1,하차역ID1,교통사업자ID1," <<
				"교통수단코드2,버스노선ID2,차량ID2,통행거리2,승차일시2,하차일시2,승차역ID2,하차역ID2,교통사업자ID2," <<
				"교통수단코드3,버스노선ID3,차량ID3,통행거리3,승차일시3,하차일시3,승차역ID3,하차역ID3,교통사업자ID3," <<
				"교통수단코드4,버스노선ID4,차량ID4,통행거리4,승차일시4,하차일시4,승차역ID4,하차역ID4,교통사업자ID4," <<
				"교통수단코드5,버스노선ID5,차량ID5,통행거리5,승차일시5,하차일시5,승차역ID5,하차역ID5,교통사업자ID5," <<
				"최초승차일시,최초승차역ID,최초승차버스노선ID,최종하차일시,최종하차역ID,최종하차버스노선ID,총이용객수," <<
				"총이용금액,총통행거리,총탑승시간,총소요시간,요일" <<
			endl;
		for (auto n : data)
		{
			n->print(fout);
		}
}

