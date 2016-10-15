/*
 * synergy -- mouse and keyboard sharing utility
 * Copyright (C) 2016 Symless Inc.
 * 
 * This package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * found in the file LICENSE that should have accompanied this file.
 * 
 * This package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define TEST_ENV

#include "shared/SerialKey.h"

#include "test/global/gtest.h"

TEST(SerialKeyTests, decode_empty_returnEmptyString)
{
	SerialKey serial("");
	std::string plainText = serial.decode("");
	EXPECT_EQ(0, plainText.size());
}

TEST(SerialKeyTests, decode_invalidDigit_returnEmptyString)
{
	SerialKey serial("");
	std::string plainText = serial.decode("MOCKZ");
	EXPECT_EQ(0, plainText.size());
}

TEST(SerialKeyTests, decode_validSerial_returnPlainText)
{
	SerialKey serial("");
	std::string plainText = serial.decode("53796E6572677920726F636B7321");
	EXPECT_EQ("Synergy rocks!", plainText);
}

TEST(SerialKeyTests, parse_noParty_invalid)
{
	SerialKey serial("");
	serial.parse("MOCK");
	EXPECT_FALSE(serial.isValid(0));
}

TEST(SerialKeyTests, parse_invalidPartsLenghth_invalid)
{
	SerialKey serial("");
	serial.parse("{Synergy;Rocks}");
	EXPECT_FALSE(serial.isValid(0));
}

TEST(SerialKeyTests, parse_validV1Serial_valid)
{
	SerialKey serial("");
	serial.parse("{v1;basic;Bob;1;email;company name;0;86400}");
	EXPECT_EQ(true, serial.isValid(0));
	EXPECT_EQ(kBasic, serial.edition());
	EXPECT_FALSE(serial.isExpired(0));
	EXPECT_EQ(true, serial.daysLeft(0));
	EXPECT_EQ(true, serial.isExpiring(1));
}

TEST(SerialKeyTests, parse_validV2Serial_valid)
{
	SerialKey serial("");
	serial.parse("{v2;trial;pro;Bob;1;email;company name;0;86400}");
	EXPECT_EQ(true, serial.isValid(0));
	EXPECT_EQ(kPro, serial.edition());
	EXPECT_FALSE(serial.isExpired(0));
	EXPECT_EQ(true, serial.daysLeft(0));
	EXPECT_EQ(true, serial.isExpiring(1));
	EXPECT_EQ(true, serial.isTrial());
}

TEST(SerialKeyTests, isValid_validV1BasicSerial_valid)
{
	// {v1;basic;Bob;1;email;company name;0;86400}
	SerialKey serial("7B76313B62617369633B426F623B313B656D61696C3B636F6D70616E79206E616D653B303B38363430307D");
	EXPECT_EQ(true, serial.isValid(0));
	EXPECT_EQ(kBasic, serial.edition());
}

TEST(SerialKeyTests, isValid_expiredV1ProSerial_valid)
{
	// {v1;pro;Bob;1;email;company name;0;86400}
	SerialKey serial("7B76313B70726F3B426F623B313B656D61696C3B636F6D70616E79206E616D653B303B38363430307D");
	EXPECT_EQ(true, serial.isValid(0));
	EXPECT_EQ(kPro, serial.edition());
}

TEST(SerialKeyTests, isValid_validV2LifetimeBasicSerial_valid)
{
	// {v2;lifetime;basic;Bob;1;email;company name;0;86400}
	SerialKey serial("7B76323B6C69666574696D653B62617369633B426F623B313B656D61696C3B636F6D70616E79206E616D653B303B38363430307D");
	EXPECT_EQ(true, serial.isValid(0));
	EXPECT_EQ(kBasic, serial.edition());
}

TEST(SerialKeyTests, isValid_validV2LifetimeProSerial_valid)
{
	// {v2;lifetime;pro;Bob;1;email;company name;0;86400}
	SerialKey serial("7B76323B6C69666574696D653B70726F3B426F623B313B656D61696C3B636F6D70616E79206E616D653B303B38363430307D");
	EXPECT_EQ(true, serial.isValid(0));
	EXPECT_EQ(kPro, serial.edition());
}

TEST(SerialKeyTests, isValid_validV2TrialBasicSerial_valid)
{
	// {v2;trial;basic;Bob;1;email;company name;0;86400}
	SerialKey serial("7B76323B747269616C3B62617369633B426F623B313B656D61696C3B636F6D70616E79206E616D653B303B38363430307D");
	EXPECT_EQ(true, serial.isTrial());
	EXPECT_EQ(true, serial.isValid(0));
	EXPECT_EQ(kBasic, serial.edition());

}

TEST(SerialKeyTests, isValid_expiredV2TrialProSerial_invalid)
{
	// {v2;trial;pro;Bob;1;email;company name;0;86400}
	SerialKey serial("7B76323B747269616C3B70726F3B426F623B313B656D61696C3B636F6D70616E79206E616D653B303B38363430307D");
	EXPECT_EQ(true, serial.isTrial());
	EXPECT_FALSE(serial.isValid(86401));
	EXPECT_EQ(kPro, serial.edition());
}

TEST(SerialKeyTests, isExpiring_validV2TrialBasicSerial_returnFalse)
{
	// {v2;trial;basic;Bob;1;email;company name;0;86400}
	SerialKey serial("7B76323B747269616C3B62617369633B426F623B313B656D61696C3B636F6D70616E79206E616D653B303B38363430307D");
	EXPECT_EQ(true, serial.isTrial());
	EXPECT_TRUE(serial.isExpiring(0));
	EXPECT_EQ(kBasic, serial.edition());
}

TEST(SerialKeyTests, isExpiring_expiringV2TrialBasicSerial_returnTrue)
{
	// {v2;trial;basic;Bob;1;email;company name;0;86400}
	SerialKey serial("7B76323B747269616C3B62617369633B426F623B313B656D61696C3B636F6D70616E79206E616D653B303B38363430307D");
	EXPECT_EQ(true, serial.isTrial());
	EXPECT_EQ(true, serial.isExpiring(1));
}

TEST(SerialKeyTests, isExpiring_expiredV2TrialBasicSerial_returnFalse)
{
	// {v2;trial;basic;Bob;1;email;company name;0;86400}
	SerialKey serial("7B76323B747269616C3B62617369633B426F623B313B656D61696C3B636F6D70616E79206E616D653B303B38363430307D");
	EXPECT_EQ(true, serial.isTrial());
	EXPECT_FALSE(serial.isExpiring(86401));
}

TEST(SerialKeyTests, isExpired_validV2TrialBasicSerial_returnFalse)
{
	// {v2;trial;basic;Bob;1;email;company name;0;86400}
	SerialKey serial("7B76323B747269616C3B62617369633B426F623B313B656D61696C3B636F6D70616E79206E616D653B303B38363430307D");
	EXPECT_EQ(true, serial.isTrial());
	EXPECT_FALSE(serial.isExpired(0));
}

TEST(SerialKeyTests, isExpired_expiringV2TrialBasicSerial_returnFalse)
{
	// {v2;trial;basic;Bob;1;email;company name;0;86400}
	SerialKey serial("7B76323B747269616C3B62617369633B426F623B313B656D61696C3B636F6D70616E79206E616D653B303B38363430307D");
	EXPECT_EQ(true, serial.isTrial());
	EXPECT_FALSE(serial.isExpired(1));
}

TEST(SerialKeyTests, isExpired_expiredV2TrialBasicSerial_returnTrue)
{
	// {v2;trial;basic;Bob;1;email;company name;0;86400}
	SerialKey serial("7B76323B747269616C3B62617369633B426F623B313B656D61696C3B636F6D70616E79206E616D653B303B38363430307D");
	EXPECT_EQ(true, serial.isTrial());
	EXPECT_EQ(true, serial.isExpired(86401));
}

TEST(SerialKeyTests, daysLeft_validExactlyOneDayV2TrialBasicSerial_returnOne)
{
	// {v2;trial;basic;Bob;1;email;company name;0;86400}
	SerialKey serial("7B76323B747269616C3B62617369633B426F623B313B656D61696C3B636F6D70616E79206E616D653B303B38363430307D");
	EXPECT_EQ(1, serial.daysLeft(0));
}

TEST(SerialKeyTests, daysLeft_validWithinOneDayV2TrialBasicSerial_returnOne)
{
	// {v2;trial;basic;Bob;1;email;company name;0;86400}
	SerialKey serial("7B76323B747269616C3B62617369633B426F623B313B656D61696C3B636F6D70616E79206E616D653B303B38363430307D");
	EXPECT_EQ(1, serial.daysLeft(1));
}

TEST(SerialKeyTests, daysLeft_expiredV2TrialBasicSerial_returnZero)
{
	// {v2;trial;basic;Bob;1;email;company name;0;86400}
	SerialKey serial("7B76323B747269616C3B62617369633B426F623B313B656D61696C3B636F6D70616E79206E616D653B303B38363430307D");
	EXPECT_EQ(0, serial.daysLeft(86401));
}
