-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Select the description from day of crime
SELECT description FROM crime_scene_reports WHERE year = 2020 AND month = 7 AND day = 28;
-- Result:
-- Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse. Interviews were conducted today with three
-- witnesses who were present at the time — each of their interview transcripts mentions the courthouse.

-- Look for license plate and activies on the day of crime in the courthouse tapes as all of the witnessess mention the courthouse in their interviews.
-- Selecting hour and minute to find something near the time of crime (10:15)
SELECT hour, minute, activity, license_plate FROM courthouse_security_logs WHERE year = 2020 AND month = 7 AND day = 28 AND hour = 10;
-- Results:
-- 10 | 8 | entrance | R3G7486
-- 10 | 14 | entrance | 13FNH73
-- 10 | 16 | exit | 5P2BI95
-- 10 | 18 | exit | 94KL13X
-- 10 | 18 | exit | 6P58WS2
-- 10 | 19 | exit | 4328GD8
-- 10 | 20 | exit | G412CB7
-- 10 | 21 | exit | L93JTIZ
-- 10 | 23 | exit | 322W7JE
-- 10 | 23 | exit | 0NTHK55
-- 10 | 35 | exit | 1106N58
-- 10 | 42 | entrance | NRYN856
-- 10 | 44 | entrance | WD5M8I6
-- 10 | 55 | entrance | V47T75I
-- Not much to go on but it might be useful later.

-- Selecting the interviews of witness for theft
SELECT name, transcript FROM interviews WHERE year = 2020 AND month = 7 AND day = 28;
-- Earlier it was said there was three witness, these three stories are most likely theirs:
-- Ruth | Sometime within ten minutes of the theft, I saw the thief get into a car in the courthouse parking lot and drive away.
--      If you have security footage from the courthouse parking lot, you might want to look for cars that left the parking lot in that time frame.
-- Eugene | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at the courthouse,
--      I was walking by the ATM on Fifer Street and saw the thief there withdrawing some money.
-- Raymond | As the thief was leaving the courthouse, they called someone who talked to them for less than a minute.
--      In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
--      The thief then asked the person on the other end of the phone to purchase the flight ticket.

-- Using Ruth's interview info to find car exits withing 10 minutes of crime
-- Using the info from earlier from the courthouse tapes, these are the cars that exited:
-- 10 | 16 | exit | 5P2BI95
-- 10 | 18 | exit | 94KL13X
-- 10 | 18 | exit | 6P58WS2
-- 10 | 19 | exit | 4328GD8
-- 10 | 20 | exit | G412CB7
-- 10 | 21 | exit | L93JTIZ
-- 10 | 23 | exit | 322W7JE
-- 10 | 23 | exit | 0NTHK55

-- Eugene saw the theif withdrawing money earleir in the morning at Fifer Street
-- Selecting the account number of people who withdrew on the date, time and location referenced
SELECT account_number, transaction_type FROM atm_transactions WHERE year = 2020 AND month = 7 AND day = 28 AND atm_location = "Fifer Street" AND transaction_type = "withdraw";
-- Results:
-- 28500762 | withdraw
-- 28296815 | withdraw
-- 76054385 | withdraw
-- 49610011 | withdraw
-- 16153065 | withdraw
-- 25506511 | withdraw
-- 81061156 | withdraw
-- 26013199 | withdraw

-- Using these account numbers to find a person ID, name, number, license and passport
SELECT * FROM people
WHERE id IN (SELECT person_id FROM bank_accounts
                WHERE account_number IN (SELECT account_number FROM atm_transactions
                                            WHERE year = 2020 AND month = 7 AND day = 28 AND atm_location = "Fifer Street" AND transaction_type = "withdraw"));
-- Results:
-- id | name | phone_number | passport_number | license_plate
-- 395717 | Bobby | (826) 555-1652 | 9878712108 | 30G67EN
-- 396669 | Elizabeth | (829) 555-5269 | 7049073643 | L93JTIZ
-- 438727 | Victoria | (338) 555-6650 | 9586786673 | 8X428L0
-- 449774 | Madison | (286) 555-6063 | 1988161715 | 1106N58
-- 458378 | Roy | (122) 555-4581 | 4408372428 | QX4YZN3
-- 467400 | Danielle | (389) 555-5198 | 8496433585 | 4328GD8
-- 514354 | Russell | (770) 555-1861 | 3592750733 | 322W7JE
-- 686048 | Ernest | (367) 555-5533 | 5773159633 | 94KL13X

-- Comparing the license plates with the results from the courthouse tapes earlier, these people are matches
-- 396669 | Elizabeth | (829) 555-5269 | 7049073643 | L93JTIZ
-- 467400 | Danielle | (389) 555-5198 | 8496433585 | 4328GD8
-- 686048 | Ernest | (367) 555-5533 | 5773159633 | 94KL13X

-- Raymond saw theif call someone after crime asking to buy plane ticket earliest as possible tomorrow (July 29)
-- Looking for early flights on this day
SELECT * FROM flights WHERE year = 2020 AND month = 7 AND day = 29 ORDER BY hour;
-- Results:
-- id | origin_airport_id | destination_airport_id | year | month | day | hour | minute
-- 36 | 8 | 4 | 2020 | 7 | 29 | 8 | 20
-- 43 | 8 | 1 | 2020 | 7 | 29 | 9 | 30
-- 23 | 8 | 11 | 2020 | 7 | 29 | 12 | 15
-- 53 | 8 | 9 | 2020 | 7 | 29 | 15 | 20
-- 18 | 8 | 6 | 2020 | 7 | 29 | 16 | 0
-- Start by assuming the supsect went on the earliest flight

-- Compare passports to see if the one of suspect was on any one of these flights
-- Start with Elizabeth
SELECT * FROM passengers WHERE passport_number = 7049073643 AND flight_id IN (SELECT id FROM flights WHERE year = 2020 AND month = 7 AND day = 29 ORDER BY hour);
-- Nothing

-- Now Daniella
SELECT * FROM passengers WHERE passport_number = 8496433585 AND flight_id IN (SELECT id FROM flights WHERE year = 2020 AND month = 7 AND day = 29 ORDER BY hour);
-- flight_id | passport_number | seat
-- 36 | 8496433585 | 7B

-- Now Ernest
SELECT * FROM passengers WHERE passport_number = 5773159633 AND flight_id IN (SELECT id FROM flights WHERE year = 2020 AND month = 7 AND day = 29 ORDER BY hour);
-- flight_id | passport_number | seat
-- 36 | 5773159633 | 4A
-- Both Ernest and Daniella were on the same earliest plane trip the 29th. 

-- The destination of flight 36 is airport 4, this is located in:
SELECT full_name, city FROM airports WHERE id = 4;
-- Heathrow Airport | London

-- Looking to see if either supsect called someone on the 28th for less than a minute
SELECT caller, receiver, duration FROM phone_calls WHERE year = 2020 AND month = 7 AND day = 28 AND duration < 60 ORDER BY duration;
-- Results:
-- (499) 555-9472 | (892) 555-8872 | 36
-- (031) 555-6622 | (910) 555-3251 | 38
-- (286) 555-6063 | (676) 555-6554 | 43
-- (367) 555-5533 | (375) 555-8161 | 45
-- (770) 555-1861 | (725) 555-3243 | 49
-- (499) 555-9472 | (717) 555-1342 | 50
-- (130) 555-0289 | (996) 555-8899 | 51
-- (338) 555-6650 | (704) 555-2131 | 54
-- (826) 555-1652 | (066) 555-9701 | 55

-- Checking if either supsect has one of these numbers
SELECT caller, receiver FROM phone_calls WHERE year = 2020 AND month = 7 AND day = 28 AND duration <= 60 AND caller IN (SELECT phone_number FROM people WHERE name = "Ernest" OR name = "Danielle");
-- Ernest called this person:
-- (367) 555-5533 | (375) 555-8161

-- FInding who has this number
SELECT * FROM people WHERE phone_number = "(375) 555-8161";
-- Result: 864400 | Berthold | (375) 555-8161 |  | 4V16VO0

-- Checking to see if Berthold was on flight 36
SELECT seat FROM passengers WHERE flight_id = 36 AND passport_number IN (SELECT passport_number FROM people WHERE phone_number = "(375) 555-8161");