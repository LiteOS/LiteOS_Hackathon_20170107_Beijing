CREATE TABLE `contacts` (
   `contactId` bigint(20) NOT NULL,
   `contactName` varchar(100) NOT NULL,
   `mobile` varchar(100) DEFAULT NULL,
   `tel` varchar(100) DEFAULT NULL,
   `deviceId` bigint(20) DEFAULT NULL,
   PRIMARY KEY (`contactId`)
 ) ENGINE=InnoDB DEFAULT CHARSET=utf8;

 CREATE TABLE `devices` (
   `deviceId` bigint(20) NOT NULL,
   `deviceSerialNo` varchar(200) NOT NULL,
   `devicePos` varchar(1000) NOT NULL,
   `userId` bigint(20) NOT NULL,
   PRIMARY KEY (`deviceId`)
 ) ENGINE=InnoDB DEFAULT CHARSET=utf8;

 CREATE TABLE `users` (
   `userId` bigint(20) NOT NULL,
   `userName` varchar(200) NOT NULL,
   `userPasswd` varchar(200) NOT NULL,
   `userCNName` varchar(100) DEFAULT NULL,
   `mobile` varchar(100) DEFAULT NULL,
   `tel` varchar(100) DEFAULT NULL,
   PRIMARY KEY (`userId`)
 ) ENGINE=InnoDB DEFAULT CHARSET=utf8;