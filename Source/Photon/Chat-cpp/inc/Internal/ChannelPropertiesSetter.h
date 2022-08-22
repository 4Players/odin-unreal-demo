/* Exit Games Photon Chat - C++ Client Lib
 * Copyright (C) 2004-2022 Exit Games GmbH. All rights reserved.
 * http://www.photonengine.com
 * mailto:developer@photonengine.com
 */

#pragma once

namespace ExitGames
{
	namespace Chat
	{
		class Client;

		namespace Internal
		{
			class ChannelPropertiesSetter
			{
				static void readProperties(Channel& channel, const Common::Dictionary<Common::Object, Common::Object>& props);
				friend class Chat::Client;
			};
		}
	}
}