/*
 * TRIPCHAIN.h
 *
 *  Created on: 2018. 3. 19.
 *      Author: lsh
 */

#ifndef SRC_TRIPCHAIN_TRIPCHAIN_H_
#define SRC_TRIPCHAIN_TRIPCHAIN_H_
#include <list>
#include "TripChainData.h"
class TRIPCHAIN {
public:
	TRIPCHAIN();
	TRIPCHAIN(sql::ResultSet* res);
	virtual ~TRIPCHAIN();
	void print(ostream* fout);	// 출력 함수

	std::list<TRIPCHAINDATA*> data;
};

#endif /* SRC_TRIPCHAIN_TRIPCHAIN_H_ */
