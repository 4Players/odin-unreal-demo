/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2022 Exit Games GmbH. All rights reserved.
 * http://www.photonengine.com
 * mailto:developer@photonengine.com
 */

#include "LoadBalancing-cpp/inc/SendDirectOptions.h"

/** @file LoadBalancing-cpp/inc/SendDirectOptions.h */

namespace ExitGames
{
	namespace LoadBalancing
	{
		using namespace Common;

		/** @class SendDirectOptions
		   This class aggregates the various optional parameters that can be passed to Client::sendDirect().
		   @sa Client::sendDirect() */

		/**
		   Constructor: Creates a new instance with the specified parameters.
		   @param targetPlayers see setTargetPlayers() - optional, defaults to an empty JVector<int>.
		   @param fallbackRelay see setFallbackRelay() - optional, defaults to false.
		   @param receiverGroup see setReceiverGroup() - optional, defaults to Lite::ReceiverGroup::OTHERS. */
		SendDirectOptions::SendDirectOptions(const JVector<int>& targetPlayers, bool fallbackRelay, nByte receiverGroup)
			: mTargetPlayers(targetPlayers)
			, mFallbackRelay(fallbackRelay)
			, mReceiverGroup(receiverGroup)
		{
		}
		
		/**
		   Destructor. */
		SendDirectOptions::~SendDirectOptions(void)
		{
		}
		
		/**
		   Copy-Constructor: Creates a new instance that is a deep copy of the argument instance.
		   @param toCopy The instance to copy. */
		SendDirectOptions::SendDirectOptions(const SendDirectOptions& toCopy)
		{
			*this = toCopy;
		}
		
		/**
		   operator=.
	   
		   Makes a deep copy of its right operand into its left operand.
	   
		   This overwrites old data in the left operand. */
		SendDirectOptions& SendDirectOptions::operator=(const SendDirectOptions& toCopy)
		{
			mTargetPlayers = toCopy.mTargetPlayers;
			mFallbackRelay = toCopy.mFallbackRelay;
			mReceiverGroup = toCopy.mReceiverGroup;

			return *this;
		}
		
		/**
		   @returns the currently set target players
		   @sa
		   setTargetPlayers() */
		const JVector<int>& SendDirectOptions::getTargetPlayers(void) const
		{
			return mTargetPlayers;
		}

		/**
		   Sets the target players.
		   @details
		   Set this to the player numbers of the clients, which should receive the event. The default value when not setting anything is an empty JVector and equivalent to a JVector that consists of the player numbers of all clients inside the room except for the sending client
		   itself.
		   Player Numbers that do not correspond to any active player inside the room get ignored. 
		   @note
		   The options setTargetPlayers() and setReceiverGroup() provide alternative ways of specifying the receivers of an event and can not be combined with each other.
		   @note
		   If getTargetPlayers() evaluates to a non-empty JVector, then the value for the target players gets used and the value for the receiver group gets ignored.
		   @note
		   Else the  value for the receiver group gets used.
		   @param targetPlayers either an empty JVector (to reset the value of the option to the default) or an array of integer values that correspond to the player numbers of the intended receivers
		   @returns a reference to the instance on which it was called to allow for chaining multiple setter calls
		   @sa
		   getTargetPlayers() */
		SendDirectOptions& SendDirectOptions::setTargetPlayers(const JVector<int>& targetPlayers)
		{
			mTargetPlayers = targetPlayers;
			return *this;
		}
		
		/**
		   @returns true if the %Photon game server that hosts the room should be used as a fallback relay (by an automatic call to opRaiseEvent()) for all specified receivers to which no direct connection exists, false otherwise
		   @sa
		   setFallbackRelay() */
		bool SendDirectOptions::getFallbackRelay(void) const 
		{
			return mFallbackRelay;
		}
		
		/**
		   Turns the fallback relay option on or off.
		   @details
		   When the fallback relay option is turned on, then the message is relayed through the game server via opRaiseEvent() for all specified receivers to which no direct connection exists.
		   When it is turned off, then it is only sent to recipients to which a direct connection exists and not to any other recipients.
		   sendDirect() returns the number of recipients to which the message could be sent out.
		   If the fallback relay option is turned on, then this number includes the recipients for which the fallback relay has been used.
		   @param fallbackRelay true if the %Photon game server that hosts the room should be used as a fallback relay (by an automatic call to opRaiseEvent()) for all specified receivers to which no direct connection exists, false otherwise
		   @returns a reference to the instance on which it was called to allow for chaining multiple setter calls
		   @sa
		   getFallbackRelay() */
		SendDirectOptions& SendDirectOptions::setFallbackRelay(bool fallbackRelay)
		{
			mFallbackRelay = fallbackRelay;
			return *this;
		}
		
		/**
		   @returns the currently set receiver group
		   @sa
		   setReceiverGroup() */
		nByte SendDirectOptions::getReceiverGroup(void) const
		{
			return mReceiverGroup;
		}

		/**
		   Sets the receiver group.
		   @details
		   Set this to one of the values from Lite::ReceiverGroup. The default value when not setting anything is Lite::ReceiverGroup::OTHERS.
		   @note
		   The options setTargetPlayers() and setReceiverGroup() provide alternative ways of specifying the receivers of an event and can not be combined with each other.
		   @note
		   If getTargetPlayers() evaluates to a non-empty JVector, then the value for the target players gets used and the value for the receiver group gets ignored.
		   @note
		   Else the  value for the receiver group gets used.
		   @param receiverGroup needs to be one of the values from Lite::ReceiverGroup
		   @returns a reference to the instance on which it was called to allow for chaining multiple setter calls
		   @sa
		   getReceiverGroup() */
		SendDirectOptions& SendDirectOptions::setReceiverGroup(nByte receiverGroup)
		{
			mReceiverGroup = receiverGroup;
			return *this;
		}

		JString& SendDirectOptions::toString(JString& retStr, bool withTypes) const
		{
			return retStr += JString(L"{targetPlayers=") + getTargetPlayers().toString() + L", fallbackRelay=" + getFallbackRelay() + L", receiverGroup=" + getReceiverGroup() + L"}";
		}
	}
}