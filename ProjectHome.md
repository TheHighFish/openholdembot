## ATTENTION -- new project hosting ##
GoogleCode announced the termination of its prohect hosting service:
http://google-opensource.blogspot.de/2015/03/farewell-to-google-code.html
Therefore we had to reorganize OpenHoldems development a bit.
  * Our discussion forums: http://maxinmontreal.com/forums/
  * Download page: https://sites.google.com/site/openholdempokerbot/downloads/
  * Soure code repository: to be announced soon.

## Description ##
OpenHoldem is an open source screen scraping framework and programmable logic engine for the online Texas Holdem poker game.

This framework does not include automated collusion capabilities.

It does include:
  * A parameter driven engine for screen scraping and interpreting game states (Table Maps)
  * A logic engine for making poker decisions based on the game states
  * A simplistic scripting language for describing how these poker decisions should be made
  * Built in support for OpenPPL (PPL = Poker Programming Language)
  * Various interface mechanisms allowing for the creation of decision logic via alternative means (C++, Perl, etc)
  * An engine for applying the poker decision to the casino table (Autoplayer)

The project would like to thank the following open source projects for their contributions to OpenHoldem:
  * Spirit parser library; http://www.boost.org
  * HScrollListBox; Nebula Technologies, Inc.; http://www.nebutech.com
  * Dynamic window resizng class; Paul DiLascia; MSDN Magazine -- July 2001
  * PokerEval library; https://gna.org/projects/pokersource/
  * Hashing algorithm from Bob Jenkins; http://burtleburtle.net/bob/hash/doobs.html
  * Visual Leak Detector; http://dmoulding.googlepages.com/vld
  * Normalized random distribution Java class; Indiana; http://pokerai.org/pj2/index.php
  * Scintilla source code editing component; http://www.scintilla.org
  * Scintilla MFC C++ wrapper class; Horst Brückner; http://www.codeproject.com/editctrl/scintillawnd.asp?select=1475770
  * Progress Indicator Dialog; P. GopalaKrishna; http://www.codeproject.com/win32/UPDialog.asp
  * Netscape-style preferences dialog; Chris Losinger; http://www.codeproject.com/KB/tabs/saprefs.aspx
  * Fast median search algorithm; Nicolas Devillard; Niklas Wirth; http://ndevilla.free.fr/median/median/index.html
  * A lightweight RPC library based on XML and HTTP; http://xmlrpc-c.sourceforge.net/
  * A string-CRC-algorithm found at http://www.kernel.org/doc/Documentation/pcmcia/crc32hash.c
  * MD5-checksum for files; Shaheer Abdulrahiman & "Napalm"; http://www.codeproject.com/Articles/32202/MD5-File-Creation-and-Verification & http://www.rohitab.com/discuss/topic/21740-md5-hashing/
  * CString-hashtable example; Sam NG; http://www.codeproject.com/Articles/13458/CMap-How-to

## The History of the WinHoldem/OpenHoldem Bot Platforms ##
**Reproduced from the original post by Matrix here: http://www.maxinmontreal.com/forums/viewtopic.php?p=28468#p28468**

For the first couple of years of its existence WH (WinHoldEm) was actively developed and developed an enthusiastic following. Although it was a commercial product Ray Bornert put in an astonishing amount of time answering queries on the WH BBS, and seemed to respond to opinions and requests there pretty quickly. One of the features he introduced were private forums (fora) which groups could request and then run their own discussions and developments within them. These must have been problem children for him. because there was a measure of falling-out, squabbling and bad blood within them. The biggest and most active of these was the infamous Lab1.

The first sign of Ray's movement away from the total nurture and support role was the introduction of Winscrape, a utility to enable the creation of profiles (Table maps). In principle WH users were then not completely dependent on him to generate table maps. This was probably motivated by the increasing tendency of pokersites to change their client display slightly at frequent intervals, and maintaining table maps must have been an increasing burden for Ray. After a period he then stopped creating table maps altogether, and threw the users onto their own abilities with Winscrape. A market economy around them was supposed to develop.

There were external WH groups by this time. Ray had closed down the labs, saying that he did not want to be in the position of one who could be accused of spying on their work, and forced them to find alternative homes. The only one which did this permanently and successfully was Lab1; ending up on a host run by a mysterious presence called MaxinMontreal. The external market economy for table maps did not work very well, because Ray had put no mechanism in WH to enable maps to be given an equivalent of DRM, and piracy and strife soon occurred.

Whilst all this was happening there was a third significant development - PBWC. Ray had always been more evangelical than commercial with regard to WH, and he shifted his energies to organising a pokerbot/human championship. Support and development of WH stagnated for a year, at least by comparison with the situation before.

These three threads combined to generate a lot of dissatisfaction amongst the dedicated WH users. The focus of this was in Lab1. The practical result was that a certain element produced a WH functional clone to seed an open source project called OpenHoldem. An open source equivalent of Winscrape followed.

This gamble seems to have paid off. There has always been a dichotomy in botters between their interests in collaborating to develop techniques, and their interest in protecting the final commercial results - the bots. Lab1 helped enormously by making their table maps non-commercial. This established the precedent that the infrastructure for a bot was a community responsibility and development. People seem to have accepted this as a good working model.

As to the hatred - the major OH protagonists acknowledge their debt to Ray for WH, and the only comment that Ray has made about OH is that you cannot compete with open source.

## License ##
GPL v3