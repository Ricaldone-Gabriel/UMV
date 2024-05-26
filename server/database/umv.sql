-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Creato il: Mag 26, 2024 alle 23:01
-- Versione del server: 10.4.32-MariaDB
-- Versione PHP: 8.2.12

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `umv`
--

-- --------------------------------------------------------

--
-- Struttura della tabella `enviroment`
--

CREATE TABLE `enviroment` (
  `ID` int(11) NOT NULL,
  `UMVId` int(11) NOT NULL,
  `Temperature` float NOT NULL,
  `Humidity` float NOT NULL,
  `Pressure` float NOT NULL,
  `Time` datetime NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;

-- --------------------------------------------------------

--
-- Struttura della tabella `umv`
--

CREATE TABLE `umv` (
  `ID` int(11) NOT NULL,
  `Nation` text NOT NULL,
  `Version` varchar(8) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;

--
-- Dump dei dati per la tabella `umv`
--

INSERT INTO `umv` (`ID`, `Nation`, `Version`) VALUES
(1, 'Italy', '1');

--
-- Indici per le tabelle scaricate
--

--
-- Indici per le tabelle `enviroment`
--
ALTER TABLE `enviroment`
  ADD PRIMARY KEY (`ID`),
  ADD KEY `UMVId` (`UMVId`);

--
-- Indici per le tabelle `umv`
--
ALTER TABLE `umv`
  ADD PRIMARY KEY (`ID`);

--
-- AUTO_INCREMENT per le tabelle scaricate
--

--
-- AUTO_INCREMENT per la tabella `enviroment`
--
ALTER TABLE `enviroment`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT per la tabella `umv`
--
ALTER TABLE `umv`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;

--
-- Limiti per le tabelle scaricate
--

--
-- Limiti per la tabella `enviroment`
--
ALTER TABLE `enviroment`
  ADD CONSTRAINT `UMVId` FOREIGN KEY (`UMVId`) REFERENCES `umv` (`ID`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
