/*
 * CPW Woodpecker Server
 * Copyright (C) 2021 CPW Online support@cpwonline.org
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>

#include "gtest/gtest.h"

#include "core/woodpecker_server.h"
#include "extras/session.h"
#include "tools/sessions_handler.h"

using namespace CPW;

class TestGen : public ::testing::Test
{
    protected:
        void SetUp() override;
        void TearDown() override;

        Core::WoodpeckerServer *TestObj_;
};

//-----------------------------------------------------------------------------

void TestGen::SetUp()
{
    TestObj_ = new Core::WoodpeckerServer(8080);
}

void TestGen::TearDown()
{
    delete TestObj_;
}

//-----------------------------------------------------------------------------

TEST_F(TestGen, BasicTest)
{
    ASSERT_NE(nullptr, &TestObj_->get_port());
    ASSERT_NE(nullptr, TestObj_->get_server_params());
    ASSERT_NE(nullptr, TestObj_->get_handler_factory());
}

TEST_F(TestGen, SesionTest)
{
    CPW::Extras::Session session;
    ASSERT_NE("", session.get_id());
    std::cout << "- Session ID: " << session.get_id() << std::endl;
}

TEST_F(TestGen, SesionHandlerTest)
{
    CPW::Tools::SessionsHandler sessions;
    for(int a = 0; a < 3; a++)
    {
        auto session = sessions.CreateSession_("", "/", 3600);
        ASSERT_NE("", session.get_id());
        std::cout << "- Session ID (Session handler): " << session.get_id() << std::endl;
    }
}

//-----------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
