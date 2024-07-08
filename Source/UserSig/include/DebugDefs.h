//  Copyright © 2022 tencent. All rights reserved.

#ifndef USERSIG_DEBUGDEFS_H_
#define USERSIG_DEBUGDEFS_H_

static const int SDKAppID = 1600043897;

/**
 *
 * Involving SECRETKEY in client code is not a good idea. In this way, SECRETKEY may be easily decompiled and reversed,
 * and if your key is leaked, attackers can steal your Tencent Cloud traffic. Therefore, this method is only suitable
 * for the local execution and debugging of the demo.
 *
 */
static const char* SECRETKEY = "97e6f4f0100954b752a8ca4eda688ccaee980c574dc965322a202a35baef9e2e";

/**
 * Signature expired time in second.
 *
 * Default：7 x 24 x 60 x 60 = 604800 = 7Days
 */
static const int EXPIRETIME = 604800;

#endif  // USERSIG_DEBUGDEFS_H_
