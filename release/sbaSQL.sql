-- --------------------------------------------------------
-- 主机:                           127.0.0.1
-- 服务器版本:                        8.0.40 - MySQL Community Server - GPL
-- 服务器操作系统:                      Win64
-- HeidiSQL 版本:                  12.8.0.6908
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;


-- 导出 sba 的数据库结构
CREATE DATABASE IF NOT EXISTS `sba` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci */ /*!80016 DEFAULT ENCRYPTION='N' */;
USE `sba`;

-- 导出  表 sba.nodedata 结构
CREATE TABLE IF NOT EXISTS `nodedata` (
  `receiveTime` bigint DEFAULT NULL,
  `airPressure` int DEFAULT NULL,
  `lumen` int DEFAULT NULL,
  `humidity` int DEFAULT NULL,
  `Nodename` text
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- 正在导出表  sba.nodedata 的数据：~2 rows (大约)
REPLACE INTO `nodedata` (`receiveTime`, `airPressure`, `lumen`, `humidity`, `Nodename`) VALUES
	(0, 0, 0, 0, 'ESP0'),
	(23, 23, 23, 23, 'ESP0');

-- 导出  表 sba.nodestatus 结构
CREATE TABLE IF NOT EXISTS `nodestatus` (
  `createTime` bigint DEFAULT NULL,
  `Nodename` text
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- 正在导出表  sba.nodestatus 的数据：~1 rows (大约)
REPLACE INTO `nodestatus` (`createTime`, `Nodename`) VALUES
	(0, 'ESP0');

/*!40103 SET TIME_ZONE=IFNULL(@OLD_TIME_ZONE, 'system') */;
/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IFNULL(@OLD_FOREIGN_KEY_CHECKS, 1) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40111 SET SQL_NOTES=IFNULL(@OLD_SQL_NOTES, 1) */;
