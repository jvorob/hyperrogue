string help;

#ifndef NOLAMBDAS
function<void()> help_delegate;
#endif

string buildHelpText() {
  DEBB(DF_GRAPH, (debugfile,"buildHelpText\n"));

#ifdef ROGUEVIZ  
  if(rogueviz::on) return rogueviz::makehelp();
#endif
  
  string h;
  h += XLAT("Welcome to HyperRogue");
#ifdef ANDROID  
  h += XLAT(" for Android");
#endif
#ifdef IOS
  h += XLAT(" for iOS");
#endif
  h += XLAT("! (version %1)\n\n", VER);
  
  h += XLAT(
    "You have been trapped in a strange, non-Euclidean world. Collect as much treasure as possible "
    "before being caught by monsters. The more treasure you collect, the more "
    "monsters come to hunt you, as long as you are in the same land type. The "
    "Orbs of Yendor are the ultimate treasure; get at least one of them to win the game!"
    );
  h += XLAT(" (press ESC for some hints about it).");
  h += "\n\n";
  
  h += XLAT(
    "You can fight most monsters by moving into their location. "
    "The monster could also kill you by moving into your location, but the game "
    "automatically cancels all moves which result in that.\n\n"
    );
    
  h += XLAT(
    "There are many lands in HyperRogue. Collect 10 treasure "
    "in the given land type to complete it; this enables you to "
    "find the magical Orbs of this land, and in some cases "
    "get access to new lands. At 25 treasures "
    "this type of Orbs starts appearing in other lands as well. Press 'o' to "
    "get the details of all the Lands.\n\n");
  h += "\n\n";
    
#ifdef MOBILE
  h += XLAT(
    "Usually, you move by touching somewhere on the map; you can also touch one "
    "of the four buttons on the map corners to change this (to scroll the map "
    "or get information about map objects). You can also touch the "
    "numbers displayed to get their meanings.\n"
    );
#else
  h += XLAT(
    "Move with mouse, num pad, qweadzxc, or hjklyubn. Wait by pressing 's' or '.'. Spin the world with arrows, PageUp/Down, and Home/Space. "
    "To save the game you need an Orb of Safety. Press 'v' for the main menu (configuration, special modes, etc.), ESC for the quest status.\n\n"
    );
  h += XLAT(
    "You can right click any element to get more information about it.\n\n"
    );
  h += XLAT("(You can also use right Shift)\n\n");
#endif
  h += XLAT("See more on the website: ") 
    + "http//roguetemple.com/z/hyper/\n\n";
  
#ifdef TOUR
  h += XLAT("Try the Tutorial to help with understanding the "
    "geometry of HyperRogue (menu -> special modes).\n\n");
#endif
  
  h += XLAT("Still confused? Read the FAQ on the HyperRogue website!\n\n");
  
  return h;
  }

string buildCredits() {
  string h;
  h += XLAT("game design, programming, texts and graphics by Zeno Rogue <zeno@attnam.com>\n\n");
  if(lang() != 0)
    h += XLAT("add credits for your translation here");
#ifndef NOLICENSE
  h += XLAT(
    "released under GNU General Public License version 2 and thus "
    "comes with absolutely no warranty; see COPYING for details\n\n"
    );
#endif
  h += XLAT(
    "special thanks to the following people for their bug reports, feature requests, porting, and other help:\n\n%1\n\n",
    "Konstantin Stupnik, ortoslon, chrysn, Adam Borowski, Damyan Ivanov, Ryan Farnsley, mcobit, Darren Grey, tricosahedron, Maciej Chojecki, Marek ÄŚtrnĂˇct, "
    "wonderfullizardofoz, Piotr MigdaĹ‚, tehora, Michael Heerdegen, Sprite Guard, zelda0x181e, Vipul, snowyowl0, Patashu, phenomist, Alan Malloy, Tom Fryers, Sinquetica, _monad, CtrlAltDestroy, jruderman"
    );
#ifdef EXTRALICENSE
  h += EXTRALICENSE;
#endif
#ifndef MOBILE
  h += XLAT(
    "\n\nSee sounds/credits.txt for credits for sound effects"
    );
  #endif
  if(musiclicense != "") h += musiclicense;
  return h;
  }

string pushtext(stringpar p) {
  string s = XLAT(
    "\n\nNote: when pushing %the1 off a heptagonal cell, you can control the pushing direction "
    "by clicking left or right half of the heptagon.", p);
#ifndef MOBILE
  s += XLAT(" With the keyboard, you can rotate the view for a similar effect (Page Up/Down).");
#endif
  return s;
  }

string princedesc() {
  if(princessgender() == GEN_M)
    return XLAT("Apparently a prince is kept locked somewhere, but you won't ever find him in this hyperbolic palace. ");
  else
    return XLAT("Apparently a princess is kept locked somewhere, but you won't ever find her in this hyperbolic palace. ");
  }

string helptitle(string s, int col) {
  return "@" + its(col) + "\t" + s + "\n";
  }

string princessReviveHelp() {
  string h = "\n\n" +
    XLAT("Killed %1 can be revived with Orb of the Love, after you collect 20 more $$$.", moPrincess);
  if(princess::reviveAt)
    h += "\n\n" +
    XLAT("%The1 will be revivable at %2 $$$", moPrincess, its(princess::reviveAt));
  return h;
  }

void describeOrb(string& help, const orbinfo& oi) {
  eOrbLandRelation olr = getOLR(oi.orb, cwt.c->land);
  eItem tr = treasureType(oi.l);
  help += "\n\n" + XLAT(olrDescriptions[olr], cwt.c->land, tr, treasureType(cwt.c->land));
  int t = items[tr] * landMultiplier(oi.l);
  if(t >= 25)
  if(olr == olrPrize25 || olr == olrPrize3 || olr == olrGuest || olr == olrMonster || olr == olrAlways) {
    help += XLAT("\nSpawn rate (as prize Orb): %1%/%2\n", 
      its(int(.5 + 100 * orbprizefun(t))),
      its(oi.gchance));
    }
  if(t >= 10)
  if(olr == olrHub) {
    help += XLAT("\nSpawn rate (in Hubs): %1%/%2\n", 
      its(int(.5 + 100 * orbcrossfun(t))),
      its(oi.gchance));
    }
  }

string generateHelpForItem(eItem it) {

   string help = helptitle(XLATN(iinf[it].name), iinf[it].color);
   
   help += XLAT(iinf[it].help);
   
   if(it == itSavedPrincess || it == itOrbLove)
     help += princessReviveHelp();
     
   if(it == itTrollEgg)
     help += XLAT("\n\nAfter the Trolls leave, you have 750 turns to collect %the1, or it gets stolen.", it);
   
   if(it == itIvory || it == itAmethyst || it == itLotus || it == itMutant) {
     help += XLAT(
       "\n\nEasy %1 might disappear when you collect more of its kind.", it);
     if(it != itMutant) help += XLAT(
       " You need to go deep to collect lots of them.");
     }
     
#ifdef MOBILE
   if(it == itOrbSafety)
     help += XLAT("This might be very useful for devices with limited memory.");
#else
   if(it == itOrbSafety)
     help += XLAT("Thus, it is potentially useful for extremely long games, which would eat all the memory on your system otherwise.\n");
#endif

   if(isRangedOrb(it)) {
     help += XLAT("\nThis is a ranged Orb. ");
#ifdef ISMOBILE   
     if(vid.shifttarget&2)
       help += XLAT("\nRanged Orbs can be targeted by long touching the desired location.");
     else
       help += XLAT("\nRanged Orbs can be targeted by touching the desired location.");
#else
     if(vid.shifttarget&1)
       help += XLAT("\nRanged Orbs can be targeted by shift-clicking the desired location. "
     else
       help += XLAT("\nRanged Orbs can be targeted by clicking the desired location. ");
     help += "You can also scroll to the desired location and then press 't'.");
#endif
     help += XLAT("\nYou can never target cells which are adjacent to the player character, or ones out of the sight range.");
     }

#ifdef MOBILE
   if(it == itGreenStone)
     help += XLAT("You can touch the Dead Orb in your inventory to drop it.");
#else
   if(it == itGreenStone)
     help += XLAT("You can press 'g' or click them in the list to drop a Dead Orb.");
#endif
   if(it == itOrbLightning || it == itOrbFlash)
     help += XLAT("\n\nThis Orb is triggered on your first attack or illegal move.");
   if(it == itOrbShield)
     help += XLAT("\n\nThis Orb protects you from attacks, scents, and insulates you "
       "from electricity. It does not let you go through deadly terrain, but "
       "if you are attacked with fire, it lets you stay in place in it.");
  if(it == itOrbEmpathy) {
    int cnt = 0;
    for(int i=0; i<ittypes; i++) {
      eItem it2 = eItem(i);
      if(isEmpathyOrb(it2)) {
        help += XLAT(cnt ? ", %1" : " %1", it2);
        cnt++;
        }
      }
    }
  
  if(itemclass(it) == IC_ORB || it == itGreenStone || it == itOrbYendor) {
    for(int i=0; i<ORBLINES; i++) {
      const orbinfo& oi(orbinfos[i]);
      if(oi.orb == it) describeOrb(help, oi);
      }
    }
  
  if(itemclass(it) == IC_TREASURE) {
    for(int i=0; i<ORBLINES; i++) {
      const orbinfo& oi(orbinfos[i]);
      if(treasureType(oi.l) == it) {
        if(oi.gchance > 0) {
          help += XLAT("\n\nOrb unlocked: %1", oi.orb);
          describeOrb(help, oi);
          }
        else if(oi.l == cwt.c->land) {
          help += XLAT("\n\nSecondary orb: %1", oi.orb);
          describeOrb(help, oi);
          }
        }
      }
    }

  return help;
  }

void addMinefieldExplanation(string& s) {

  s += XLAT(
    "\n\nOnce you collect 10 Bomberbird Eggs, "
    "stepping on a cell with no adjacent mines also reveals the adjacent cells. "
    "Collecting even more Eggs will increase the radius. Additionally, collecting "
    "25 Bomberbird Eggs will reveal adjacent cells even in your future games."
    );

  s += "\n\n";
#ifdef MOBILE
  s += XLAT("Known mines may be marked by pressing 'm'. Your allies won't step on marked mines.");
#else
  s += XLAT("Known mines may be marked by touching while in drag mode. Your allies won't step on marked mines.");
#endif
  }

string generateHelpForWall(eWall w) {

  string s = helptitle(XLATN(winf[w].name), winf[w].color);
   
  s += XLAT(winf[w].help);
  if(w == waMineMine || w == waMineUnknown || w == waMineOpen)
    addMinefieldExplanation(s);
  if(isThumper(w)) s += pushtext(w);
  if((w == waClosePlate || w == waOpenPlate) && purehepta) 
    s += "\n\n(For the heptagonal mode, the radius has been reduced to 2 for closing plates.)";
  return s;
  }

void buteol(string& s, int current, int req) {
  int siz = size(s);
  if(s[siz-1] == '\n') s.resize(siz-1);
  char buf[100]; sprintf(buf, " (%d/%d)", current, req);
  s += buf; s += "\n";
  }

string generateHelpForMonster(eMonster m) {
  string s = helptitle(XLATN(minf[m].name), minf[m].color);
  
  if(m == moPlayer) {
#ifdef TOUR
    if(tour::on)
      return s+XLAT(
        "A tourist from another world. They mutter something about the 'tutorial', "
        "and claim that they are here just to learn, and to leave without any treasures. "
        "Do not kill them!"
        );
#endif
    s += XLAT(
        "This monster has come from another world, presumably to steal our treasures. "
        "Not as fast as an Eagle, not as resilient as the guards from the Palace, "
        "and not as huge as the Mutant Ivy from the Clearing; however, "
        "they are very dangerous because of their intelligence, "
        "and access to magical powers.\n\n");
    
    if(cheater)
      s += XLAT("Actually, their powers appear god-like...\n\n");
    
    else if(!hardcore)
      s += XLAT(
       "Rogues will never make moves which result in their immediate death. "
       "Even when cornered, they are able to instantly teleport back to their "
       "home world at any moment, taking the treasures forever... but "
       "at least they will not steal anything further!\n\n"
       );
    
    if(!euclid)
      s += XLAT(
        "Despite this intelligence, Rogues appear extremely surprised "
        "by the most basic facts about geometry. They must come from "
        "some really strange world.\n\n"
        );
    
    if(shmup::on)
      s += XLAT("In the Shoot'em Up mode, you are armed with thrown Knives.");
    
    return s;
    }

  s += XLAT(minf[m].help);      
  if(m == moPalace || m == moSkeleton)
    s += pushtext(m);  
  if(m == moTroll) s += XLAT(trollhelp2);  

  if(isMonsterPart(m))
    s += XLAT("\n\nThis is a part of a monster. It does not count for your total kills.", m);

  if(isFriendly(m))
    s += XLAT("\n\nThis is a friendly being. It does not count for your total kills.", m);

  if(m == moTortoise)
    s += XLAT("\n\nTortoises are not monsters! They are just annoyed. They do not count for your total kills.", m);
  
  if(isGhost(m))
    s += XLAT("\n\nA Ghost never moves to a cell which is adjacent to another Ghost of the same kind.", m);
    
  if(m == moBat || m == moEagle)
    s += XLAT("\n\nFast flying creatures may attack or go against gravity only in their first move.", m);

  return s;
  }

string generateHelpForLand(eLand l) {
  string s = helptitle(XLATN(linf[l].name), linf[l].color);
  
  if(l == laPalace) s += princedesc();

  s += XLAT(linf[l].help);

  if(l == laMinefield) addMinefieldExplanation(s);

  s += "\n\n";
  if(l == laIce || l == laCaves || l == laDesert || l == laMotion || l == laJungle ||
    l == laCrossroads || l == laAlchemist)
      s += XLAT("Always available.\n");

  #define ACCONLY(z) s += XLAT("Accessible only from %the1.\n", z);
  #define ACCONLY2(z,x) s += XLAT("Accessible only from %the1 or %the2.\n", z, x);
  #define ACCONLYF(z) s += XLAT("Accessible only from %the1 (until finished).\n", z);
  #define TREQ(z) { s += XLAT("Treasure required: %1 $$$.\n", its(z)); buteol(s, gold(), z); }
  #define TREQ2(z,x) { s += XLAT("Treasure required: %1 x %2.\n", its(z), x); buteol(s, items[x], z); }
  
  if(l == laMirror || l == laMinefield || l == laPalace ||
    l == laOcean || l == laLivefjord || l == laZebra || l == laWarpCoast || l == laWarpSea ||
    l == laReptile || l == laIvoryTower)
      TREQ(R30)

  
  if(isCoastal(l)) 
    s += XLAT("Coastal region -- connects inland and aquatic regions.\n");
    
  if(isPureSealand(l)) 
    s += XLAT("Aquatic region -- accessible only from coastal regions and other aquatic regions.\n");
    
  if(l == laWhirlpool) ACCONLY(laOcean)
  if(l == laRlyeh) ACCONLYF(laOcean)
  if(l == laTemple) ACCONLY(laRlyeh)  
  if(l == laClearing) ACCONLY(laOvergrown)  
  if(l == laHaunted) ACCONLY(laGraveyard)  
  if(l == laPrincessQuest) ACCONLY(laPalace)
  if(l == laMountain) ACCONLY(laJungle)
  if(l == laCamelot) ACCONLY2(laCrossroads, laCrossroads3)
  
  if(l == laDryForest || l == laWineyard || l == laDeadCaves || l == laHive || l == laRedRock ||
    l == laOvergrown || l == laStorms || l == laWhirlwind || l == laRose ||
    l == laCrossroads2 || l == laRlyeh)
      TREQ(R60)
    
  if(l == laReptile) TREQ2(U10, itElixir)
  if(l == laEndorian) TREQ2(U10, itIvory)
  if(l == laKraken) TREQ2(U10, itFjord)
  if(l == laBurial) TREQ2(U10, itKraken)

  if(l == laDungeon) TREQ2(U5, itIvory)
  if(l == laDungeon) TREQ2(U5, itPalace)
  if(l == laMountain) TREQ2(U5, itIvory)
  if(l == laMountain) TREQ2(U5, itRuby)
    
  if(l == laPrairie) TREQ(R90)
  if(l == laBull) TREQ(R90)
  if(l == laCrossroads4) TREQ(R200)
  if(l == laCrossroads5) TREQ(R300)
  
  if(l == laGraveyard || l == laHive) {
    s += XLAT("Kills required: %1.\n", "100");
    buteol(s, tkills(), R100);
    }
  
  if(l == laDragon) {
    s += XLAT("Different kills required: %1.\n", "20");
    buteol(s, killtypes(), R20);
    }
  
  if(l == laTortoise) ACCONLY(laDragon)
  if(l == laTortoise) s += XLAT("Find a %1 in %the2.", itBabyTortoise, laDragon);

  if(l == laHell || l == laCrossroads3) {
    s += XLAT("Finished lands required: %1 (collect %2 treasure)\n", "9", its(R10));
    buteol(s, orbsUnlocked(), 9);
    }
  
  if(l == laCocytus || l == laPower) TREQ2(U10, itHell)
  if(l == laRedRock) TREQ2(U10, itSpice)
  if(l == laOvergrown) TREQ2(U10, itRuby)
  if(l == laClearing) TREQ2(U5, itMutant)
  if(l == laCocytus) TREQ2(U10, itDiamond)
  if(l == laDeadCaves) TREQ2(U10, itGold)
  if(l == laTemple) TREQ2(U5, itStatue)
  if(l == laHaunted) TREQ2(U10, itBone)
  if(l == laCamelot) TREQ2(U5, itEmerald)
  if(l == laEmerald) {
    TREQ2(5, itFernFlower) TREQ2(5, itGold)
    s += XLAT("Alternatively: kill a %1 in %the2.\n", moVizier, laPalace);
    buteol(s, kills[moVizier], 1);
    }
  
#define KILLREQ(who, where) { s += XLAT("Kills required: %1 (%2).\n", who, where); buteol(s, kills[who], 1); }

  if(l == laPrincessQuest)
    KILLREQ(moVizier, laPalace);
    
  if(l == laElementalWall) {
    KILLREQ(moFireElemental, laDragon);
    KILLREQ(moEarthElemental, laDeadCaves);
    KILLREQ(moWaterElemental, laLivefjord);
    KILLREQ(moAirElemental, laWhirlwind);
    }
  
  if(l == laTrollheim) {
    KILLREQ(moTroll, laCaves);
    KILLREQ(moFjordTroll, laLivefjord);
    KILLREQ(moDarkTroll, laDeadCaves);
    KILLREQ(moStormTroll, laStorms);
    KILLREQ(moForestTroll, laOvergrown);
    KILLREQ(moRedTroll, laRedRock);
    }
  
  if(l == laZebra) TREQ2(U10, itFeather)
  
  if(l == laCamelot || l == laPrincessQuest)
    s += XLAT("Completing the quest in this land is not necessary for the Hyperstone Quest.");

  int rl = isRandland(l);
  if(rl == 2)
    s += XLAT("Variants of %the1 are always available in the Random Pattern Mode.", l);
  else if(rl == 1)
    s += XLAT(
      "Variants of %the1 are available in the Random Pattern Mode after "
      "getting a highscore of at least 10 %2.", l, treasureType(l));

  if(l == laPrincessQuest) {
    s += XLAT("Unavailable in the shmup mode.\n");
    s += XLAT("Unavailable in the multiplayer mode.\n");
    }
  
  if(noChaos(l))
    s += XLAT("Unavailable in the Chaos mode.\n");
  
  if(l == laWildWest)
    s += XLAT("Bonus land, available only in some special modes.\n");
  
  if(l == laWhirlpool)
    s += XLAT("Orbs of Safety always appear here, and may be used to escape.\n");

  /* if(isHaunted(l) || l == laDungeon)
    s += XLAT("You may be unable to leave %the1 if you are not careful!\n", l); */
    
  if(l == laStorms) {
    if(elec::lightningfast == 0) s += XLAT("\nSpecial conduct (still valid)\n");
    else s += XLAT("\nSpecial conduct failed:\n");
    
    s += XLAT(
      "Avoid escaping from a discharge (\"That was close\").");
    }

  if(isHaunted(l)) {
    if(survivalist) s += XLAT("\nSpecial conduct (still valid)\n");
    else s += XLAT("\nSpecial conduct failed:\n");

    s += XLAT(
      "Avoid chopping trees, using Orbs, and non-graveyard monsters in the Haunted Woods."
      );
    }
  
#ifndef ISMOBILE
  if(l == laCA)
    s += XLAT("\n\nHint: use 'm' to toggle cells quickly");
#endif

  return s;
  }

bool instat;

string turnstring(int i) {
  if(i == 1) return XLAT("1 turn");
  else return XLAT("%1 turns", its(i));
  }

void describeMouseover() {
  DEBB(DF_GRAPH, (debugfile,"describeMouseover\n"));

  cell *c = mousing ? mouseover : playermoved ? NULL : centerover;
  string out = mouseovers;
  if(!c || instat || getcstat) { }
  else if(c->wall != waInvisibleFloor) {
    out = XLAT1(linf[c->land].name);
    help = generateHelpForLand(c->land);
        
    // Celsius
    
    // if(c->land == laIce) out = "Icy Lands (" + fts(60 * (c->heat - .4)) + " C)";
    
    if(c->land == laIce || c->land == laCocytus) 
      out += " (" + fts(heat::celsius(c)) + " °C)";
    if(c->land == laDryForest && c->landparam) 
      out += " (" + its(c->landparam)+"/10)";
    if(c->land == laOcean && chaosmode)
      out += " (" + its(c->CHAOSPARAM)+"S"+its(c->SEADIST)+"L"+its(c->LANDDIST)+")";
    else if(c->land == laOcean && c->landparam <= 25) {
      if(shmup::on)
        out += " (" + its(c->landparam)+")";
      else {
        bool b = c->landparam >= tide[(turncount-1) % tidalsize];
        int t = 1;
        for(; t < 1000 && b == (c->landparam >= tide[(turncount+t-1) % tidalsize]); t++) ;
        if(b)
          out += " (" + turnstring(t) + XLAT(" to surface") + ")";
        else 
          out += " (" + turnstring(t) + XLAT(" to submerge") + ")";
        }
      }

    if(c->land == laTortoise && tortoise::seek()) out += " " + tortoise::measure(getBits(c));

    /* if(c->land == laGraveyard || c->land == laHauntedBorder || c->land == laHaunted)
      out += " (" + its(c->landparam)+")"; */
    
    if(buggyGeneration) {
      char buf[20]; sprintf(buf, " H=%d M=%d", c->landparam, c->mpdist); out += buf;
      }
    
//  if(c->land == laBarrier)
//    out += "(" + string(linf[c->barleft].name) + " / " + string(linf[c->barright].name) + ")";

    // out += "(" + its(c->bardir) + ":" + string(linf[c->barleft].name) + " / " + string(linf[c->barright].name) + ")";
    
    // out += " MD"+its(c->mpdist);

    // out += " WP:" + its(c->wparam);
    // out += " rose:" + its(rosemap[c]/4) + "." + its(rosemap[c]%4);
    // out += " MP:" + its(c->mpdist);
    // out += " cda:" + its(celldistAlt(c));
    
    /* out += " DP=" + its(celldistance(c, cwt.c));
    out += " DO=" + its(celldist(c));
    out += " PD=" + its(c->pathdist); */

    if(false) {

      out += " LP:" + itsh(c->landparam)+"/"+its(turncount);

      out += " CD:" + its(celldist(c));
      
      out += " D:" + its(c->mpdist);
      
      char zz[64]; sprintf(zz, " P%p", c); out += zz;
      // out += " rv" + its(rosedist(c));
  //  if(rosemap.count(c))
  //    out += " rv " + its(rosemap[c]/8) + "." + its(rosemap[c]%8);
  //  out += " ai" + its(c->aitmp);
      if(euclid) {
        for(int i=0; i<4; i++) out += " " + its(getEuclidCdata(c->master)->val[i]);
        out += " " + itsh(getBits(c));
        }
      else {
        for(int i=0; i<4; i++) out += " " + its(getHeptagonCdata(c->master)->val[i]);
  //  out += " " + itsh(getHeptagonCdata(c->master)->bits);
        out += " " + fts(tortoise::getScent(getBits(c)));
        }
      // itsh(getHeptagonCdata(c->master)->bits);
  //  out += " barleft: " + s0 + dnameof(c->barleft);
  //  out += " barright: " + s0 + dnameof(c->barright);
      }
    
    // char zz[64]; sprintf(zz, " P%p", c); out += zz;
    
    /* whirlwind::calcdirs(c);
    for(int i=0; i<whirlwind::qdirs; i++) 
      out += " " + its(whirlwind::dfrom[i]) + ":" + its(whirlwind::dto[i]); */
    // out += " : " + its(whirlwinddir(c));
    
    

    if(randomPatternsMode)
      out += " " + describeRPM(c->land);
      
    if(euclid && cheater) {
      if(torus) {
        out += " ("+its(decodeId(c->master))+")";
        }
      else {
        eucoord x, y;
        decodeMaster(c->master, x, y);
        out += " ("+its(short(x))+","+its(short(y))+")";
        }
      }
      
    // char zz[64]; sprintf(zz, " P%d", princess::dist(c)); out += zz;
    // out += " MD"+its(c->mpdist);
    // out += " H "+its(c->heat);
    // if(c->type != 6) out += " Z"+its(c->master->zebraval);
    // out += " H"+its(c->heat);
    
/*  // Hive debug
    if(c->land == laHive) {
      out += " [" + its(c->tmp) + " H" + its(int(c->heat));
      if(c->tmp >= 0 && c->tmp < size(buginfo) && buginfo[c->tmp].where == c) {
        buginfo_t b(buginfo[c->tmp]);
        for(int k=0; k<3; k++) out += ":" + its(b.dist[k]);
        for(int k=0; k<3; k++) 
        for(int i=0; i<size(bugqueue[k]); i++)
          if(bugqueue[k][i] == c->tmp)
            out += " B"+its(k)+":"+its(i);
        }
      out += "]";
      } */
  
    if(c->wall && 
      !((c->wall == waFloorA || c->wall == waFloorB || c->wall == waFloorC || c->wall == waFloorD) && c->item)) { 
      out += ", "; out += XLAT1(winf[c->wall].name); 
      
      if(c->wall == waRose) out += " (" + its(7-rosephase) + ")";
      
      if((c->wall == waBigTree || c->wall == waSmallTree) && c->land != laDryForest)
        help = 
          "Trees in this forest can be chopped down. Big trees take two turns to chop down.";
      else if(c->wall != waSea && c->wall != waPalace)
      if(!((c->wall == waCavefloor || c->wall == waCavewall) && c->land == laEmerald))
        help = generateHelpForWall(c->wall);
      }
    
    if(isActivable(c)) out += XLAT(" (touch to activate)");
    
    if(hasTimeout(c)) out += " [" + turnstring(c->wparam) + "]";
    
    if(isReptile(c->wall))
      out += " [" + turnstring((unsigned char) c->wparam) + "]";
  
    if(c->monst) {
      out += ", "; out += XLAT1(minf[c->monst].name); 
      if(hasHitpoints(c->monst))
        out += " (" + its(c->hitpoints)+" HP)";
      if(isMutantIvy(c))
        out += " (" + its((c->stuntime - mutantphase) & 15) + "*)";
      else if(c->stuntime)
        out += " (" + its(c->stuntime) + "*)";

      if(c->monst == moTortoise && tortoise::seek()) 
        out += " " + tortoise::measure(tortoise::getb(c));

      help = generateHelpForMonster(c->monst);
      }
  
    if(c->item && !itemHiddenFromSight(c)) {
      out += ", "; 
      out += XLAT1(iinf[c->item].name); 
      if(c->item == itBarrow) out += " (x" + its(c->landparam) + ")";
      if(c->item == itBabyTortoise && tortoise::seek()) 
        out += " " + tortoise::measure(tortoise::babymap[c]);
      if(!c->monst) help = generateHelpForItem(c->item);
      }
    
    if(isPlayerOn(c) && !shmup::on) out += XLAT(", you"), help = generateHelpForMonster(moPlayer);

    shmup::addShmupHelp(out);

    if(rosedist(c) == 1)
      out += ", wave of scent (front)";

    if(rosedist(c) == 2)
      out += ", wave of scent (back)";
    
    if(sword::at(c)) out += ", Energy Sword";
    
    if(rosedist(c) || c->land == laRose || c->wall == waRose)
      help += s0 + "\n\n" + rosedesc;
    
    if(isWarped(c) && !isWarped(c->land))
      out += ", warped";

    if(isWarped(c)) 
      help += s0 + "\n\n" + warpdesc;
    }
/*
  else if(cmode == emGraphConfig) {
    if(getcstat == 'a' && vid.sspeed > -4.99)
      out = XLAT("+5 = center instantly, -5 = do not center the map");
    else if(getcstat == 'a')
      out = XLAT("press Space or Home to center on the PC");
    else if(getcstat == 'w')
      out = XLAT("also hold Alt during the game to toggle high contrast");
    else if(getcstat == 'f')
      out = XLAT("Reduce the framerate limit to conserve CPU energy");
    }
  else if(cmode == emBasicConfig) {
    if(getcstat == 'c')
      out = XLAT("The axes help with keyboard movement");
    else if(getcstat == 'g')
      out = XLAT("Affects looks and grammar");
#ifndef MOBILE
    else if(getcstat == 's')
      out = XLAT("Config file: %1", conffile);
#endif
    else out = "";
    }
  else if(cmode == emDisplayMode) {
    if(getcstat == 'p') {
      if(autojoy) 
        out = XLAT("joystick mode: automatic (release the joystick to move)");
      if(!autojoy) 
        out = XLAT("joystick mode: manual (press a button to move)");
      }
    }
  else if(cmode == emChangeMode) {
    if(getcstat == 'h')
      out = XLAT("One wrong move and it is game over!");
    }
*/
    
  mouseovers = out;

#ifdef ROGUEVIZ
  rogueviz::describe(c);
#endif
  
  int col = linf[cwt.c->land].color;
  if(cwt.c->land == laRedRock) col = 0xC00000;

#ifndef MOBILE
  displayfr(vid.xres/2, vid.fsize,   2, vid.fsize, out, col, 8);
#endif

  if(mousey < vid.fsize * 3/2) getcstat = SDLK_F1;
  }

void showHelp() {
  cmode2 = smHelp;
  getcstat = SDLK_ESCAPE;
  if(help == "HELPFUN") {
    help_delegate();
    return;
    }

  if(help == "@") help = buildHelpText();
  
  string help2;
  if(help[0] == '@') {
    int iv = help.find("\t");
    int id = help.find("\n");
    dialog::init(help.substr(iv+1, id-iv-1), atoi(help.c_str()+1), 120, 100);
    dialog::addHelp(help.substr(id+1));
    }
  else {
    dialog::init("help", forecolor, 120, 100);
    dialog::addHelp(help);
    }
  
  if(help == buildHelpText()) dialog::addItem("credits", 'c');
  
  dialog::display();
  
  keyhandler = [] (int sym, int uni) {
    dialog::handleNavigation(sym, uni);
    if(sym == SDLK_F1 && help != "@") 
      help = "@";
    else if(uni == 'c')
      help = buildCredits();
    else if(doexiton(sym, uni))
      popScreen();
    };
  }

void gotoHelp(const string& h) {
  help = h;
  pushScreen(showHelp);
  }