//  Copyright © 2022 tencent. All rights reserved.

#ifndef USERSIG_DEBUGDEFS_H_
#define USERSIG_DEBUGDEFS_H_

static const int SDKAppID = 0;

/**
 *
 * Involving SECRETKEY in client code is not a good idea. In this way, SECRETKEY may be easily decompiled and reversed,
 * and if your key is leaked, attackers can steal your Tencent Cloud traffic. Therefore, this method is only suitable
 * for the local execution and debugging of the demo.
 *
 */
static const char* SECRETKEY = "";

/**
 * Signature expired time in second.
 *
 * Default：7 x 24 x 60 x 60 = 604800 = 7Days
 */
static const int EXPIRETIME = 604800;

#endif  // USERSIG_DEBUGDEFS_H_
