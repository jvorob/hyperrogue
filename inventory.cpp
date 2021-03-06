namespace inv {

  bool on;
  int usedup[ittypes];
  int remaining[ittypes];

  int rseed;
  bool usedForbidden;

    
  void init() {
    rseed = hrandpos();
    usedForbidden = false;
    for(int i=0; i<ittypes; i++) usedup[i] = 0;
    }
  
  static const int MIRRORED = 1000;
  static const int TESTMIRRORED = 900;
  
  int mirrorqty(eItem orb) {
    if(shmup::on && isShmupLifeOrb(orb)) 
      return 3;
    if(orb == itOrbWater) return 10;
    if(orb == itOrbSummon) return 9;
    if(orb == itOrbEmpathy) return 9;
    if(orb == itOrbMatter) return 9;
    if(orb == itOrbLuck) return 8;
    if(orb == itOrbSpace) return 7;

    if(orb == itOrbWinter) return 6;
    if(orb == itOrbLife) return 6;
    if(orb == itOrbLove) return 6;
    if(orb == itOrbRecall) return 6;
    if(orb == itOrbDigging) return 6;
    
    if(orb == itOrbTime) return 5;
    if(orb == itOrbAir) return 5;
    if(orb == itOrbFish) return 5;
    if(orb == itOrbStunning) return 5;
    if(orb == itOrbUndeath) return 5;
    if(orb == itOrb37) return 5;
    if(orb == itOrbDomination) return 5;
    if(orb == itOrbBull) return 5;
    if(orb == itOrbHorns) return 5;

    if(orb == itOrbAether) return 4;
    if(orb == itOrbInvis) return 4;
    if(orb == itOrbFire) return 4;
    if(orb == itOrbDragon) return 4;
    if(orb == itOrbIllusion) return 4;
    if(orb == itOrbDiscord) return 4;
    if(orb == itOrbBeauty) return 4;

    if(orb == itOrbMirror) return 1;
    return 3;
    }
    
  struct nextinfo { int min, real, max; };
  
  nextinfo next[ittypes];

  mt19937 invr;
  
  void sirand(int i) {
    invr.seed(i);
    }
  
  int irand(int i) {    
    return invr() % i;
    }
    
  void gainOrbs(eItem it, eItem o) {
    int qty = items[it];
    if(it == itHolyGrail) {
      remaining[itOrbIllusion] += qty;
      next[it] = {qty+1, qty+1, qty+1};
      }
    else {
      bool nextfound = false;
      if(qty >= 10) remaining[o]++;
      else next[it] = {10,10,10}, nextfound = true;
      int last = 10;
      for(int k=0; k<30 || !nextfound; k++) {
        int maxstep = 15 + 5 * k;
        if(o == itOrbMirror)
          maxstep += 5 * (k-1) * (k-2);
        else
          maxstep += (k-1) * (k-2);
        int xnext;
        if(k >= 30 || o == itOrbMirror) {
          xnext = last + maxstep/2; last = xnext-1;
          maxstep = 1;
          }
        else 
          xnext = last + 1 + irand(maxstep);
        if(xnext > qty && !nextfound) 
          next[it] = { last+1, xnext, last + maxstep }, nextfound = true;
        if(xnext <= qty) remaining[o]++; 
        last = xnext;
        }
      }
    }
  
  void gainMirrors(int qtl) {
    while(qtl > 0) qtl >>= 1, remaining[itOrbMirror]++;
    }
    
  void compute() {
    for(int i=0; i<ittypes; i++) remaining[i] = -usedup[i];
    for(int i=0; i<ittypes; i++) if(usedup[i] >= TESTMIRRORED) remaining[i] += MIRRORED;
    
    sirand(rseed);
    
    vector<pair<eItem, eItem>> itempairs;
    
    for(int k=0; k<ORBLINES; k++) {
      auto oi = orbinfos[k];
      eLand l = oi.l;
      eItem it = treasureType(l);
      eItem o = oi.orb;
      if(oi.gchance) itempairs.emplace_back(it, o);
      else if(items[it] >= 10) remaining[o]++;
      }
    
    sort(itempairs.begin(), itempairs.end());
    
    gainOrbs(itShard, itOrbMirror);
    gainOrbs(itHyperstone, itOrbMirror);
    for(auto p: itempairs)
      gainOrbs(p.first, p.second);
    if(items[itOrbYendor]) remaining[itOrbMirror]++;

    gainMirrors(items[itOrbYendor]);
    gainMirrors(items[itHolyGrail]);
    
    if(princess::reviveAt) {
      remaining[itOrbLove]++;
      int s = gold(NO_LOVE);
      int last = princess::reviveAt;
      for(int k=0;; k++) {
        int nextstep = 50 + 20 * k;
        last += nextstep;
        if(last > s) {
          next[itSavedPrincess] = {last, last, last};
          break;
          }
        else { last += nextstep; remaining[itOrbLove]++; }
        }
      }
    
    vector<eItem> offensiveOrbs = {
      itOrbFlash, itOrbLightning, itOrbPsi, itOrbThorns,
      itOrbFreedom, itOrbSword, itOrbSword2,
      itOrbHorns, itOrbDragon, itOrbStunning
      };
    const int qoff = size(offensiveOrbs);    
    for(int i=1; i<qoff; i++) swap(offensiveOrbs[i], offensiveOrbs[irand(1+i)]);
    for(int i=0; (i+1)*25 <= items[itBone]; i++) 
      remaining[offensiveOrbs[i%qoff]]++;
    
    if(items[itOrbLove] && !items[itSavedPrincess]) items[itSavedPrincess] = 1;
    
    int& r = remaining[itGreenStone];
    
    if(items[itBone] >= 10) {
      for(int i=0; i<ittypes; i++) if(i != itGreenStone) {
        r += usedup[i];
        if(usedup[i] >= TESTMIRRORED) r -= MIRRORED;
        }
      }
    
    items[itGreenStone] += r;
    usedup[itGreenStone] += r;
    r = 0;
    
    if(shmup::on) for(int i=0; i<ittypes; i++) {
      if(remaining[i] && isShmupLifeOrb(eItem(i))) {
        gainLife();
        remaining[i]--;
        usedup[i]++;
        }
      }

    items[itInventory] = 0;
    for(int i=0; i<ittypes; i++) 
      if(i != itGreenStone && i != itOrbYendor) 
        items[itInventory] += remaining[i];
    
    }
  
  map<char, eItem> orbmap;
  string orbkeys = "zfwplSetsTaMIYgCcPOWAFydLGRUuouE.,bVNhDwWZnrvhBm0123456789";
  typedef pair<int, int> pxy;
  vector<pxy> orbcoord;

  int sq(pxy p) {
    int zz = (2*p.first+p.second)*(2*p.first+p.second) + 3*p.second*p.second;
    zz *= 20; zz += abs(p.second);
    zz *= 20; zz += abs(p.first);
    zz *= 4; zz += (p.first>0)*2+(p.second>0);
    return zz;
    }

  bool plain;      

  eItem which;
  
  bool mirroring;
  
  const char* helptext = 
    "You are playing in the Orb Strategy Mode. Collecting treasure "
    "gives you access to magical Orb powers. In this mode, "
    "unlocking requirements are generally higher, and "
    "several quests and lands "
    "give you extremely powerful Orbs of the Mirror.\n";

  void show() {
  
    gamescreen(2);
    cmode = sm::CENTER;

    orbcoord.clear();
    for(int y=-3; y<=3; y++) for(int x=-4; x<=4; x++) if(x+y<=4 && x+y >= -4 && (x||y))
      orbcoord.emplace_back(x,y);
    sort(orbcoord.begin(), orbcoord.end(), [](pxy p1, pxy p2) {
      return sq(p1) < sq(p2); });
    
    ld rad = min(vid.xres, vid.yres) / 20;
    ld rad3 = int(rad * sqrt(3));
    
    compute();
    orbmap.clear();
    which = itNone;
        
    if(plain) dialog::init(XLAT(mirroring ? "mirror what?" : "inventory"), forecolor, 150, 100);

    int j = 0, oc = 6;
    for(int i=0; i<ittypes; i++) {
      eItem o = eItem(i);
      if(itemclass(o) == IC_ORB) {
        char c = orbkeys[j++];
        if(remaining[i] || usedup[i]) {
          orbmap[c] = o;
          if(plain) 
            dialog::addSelItem(XLAT1(iinf[o].name), its(remaining[i]), c);
          else {
            auto pos = orbcoord[oc++];
            ld px = vid.xres/2 + 2*rad*pos.first + rad*pos.second;
            ld py = vid.yres/2 + pos.second * rad3;
            int icol = iinf[o].color;
            if(!remaining[i]) icol = gradient(icol, 0, 0, .5, 1);
            bool gg = graphglyph();
            
            if(!hiliteclick) {
              if(gg) {
                initquickqueue();
                transmatrix V = atscreenpos(px, py, rad*2);
                drawItemType(o, NULL, V, icol, ticks/3 + i * 137, false);
                quickqueue();
                }
              
              int tcol = remaining[i] ? darkenedby(icol, 1) : 0;
  
              if(remaining[i] != 1 || !gg)
                displaystr(px, py, 2, gg?rad:rad*3/2, remaining[i] <= 0 ? "X" : remaining[i] == 1 ? "o" : its(remaining[i]), tcol, 8);
              }
            
            bool b = hypot(mousex-px, mousey-py) < rad;
            if(b) {
              getcstat = c, 
              which = o;
              }
            }
          }
        }
      }

    if(plain) {
      dialog::addBreak(750);
      dialog::addItem(XLAT("help"), SDLK_F1);
      dialog::addItem(XLAT("return to the game"), 'i');
      dialog::display();
      which = orbmap[getcstat];
      }
    else {
      
      if(which == itNone) {
        displaystr(vid.xres/2, vid.fsize*2, 2, vid.fsize*2, XLAT("Which orb to use?"), 0xC0C0C0, 8);
        }
      else {
        int icol = iinf[which].color;
        displaystr(vid.xres/2, vid.fsize*2, 2, vid.fsize*2, XLAT1(iinf[which].name), icol, 8);
        auto oi = getOrbInfo(which);
        
        if(mirroring)
          displaystr(vid.xres/2, vid.fsize*4, 2, vid.fsize, usedup[which] >= TESTMIRRORED ? XLAT("already mirrored") : XLAT("Uses to gain: %1", its(mirrorqty(which))), icol, 8);
        else {
          eItem t = treasureType(oi.l);
          string s = XLAT("Unlocked by: %1 in %2", t, oi.l);
          if(next[t].min == next[t].max)
            s += XLAT(" (next at %1)", its(next[t].min));
          else
            s += XLAT(" (next at %1 to %2)", its(next[t].min), its(next[t].max));
          displaystr(vid.xres/2, vid.fsize*4, 2, vid.fsize, s, icol, 8);
          }
        if(remaining[which] != 1)
          displaystr(vid.xres/2, vid.fsize*5, 2, vid.fsize, XLAT("Number of uses left: %1", its(remaining[which])), icol, 8);
#if ISMOBILE==0
        string hot = XLAT1("Hotkey: "); hot += getcstat;
        displaystr(vid.xres/2, vid.fsize*6, 2, vid.fsize, hot, icol, 8);
#endif
  
        eOrbLandRelation olr = getOLR(oi.orb, getPrizeLand());
        eItem tr = treasureType(oi.l);
        
        int col = 0;
        if(olr == olrDangerous) col = 0xC00000;
        if(olr == olrUseless) col = 0x808080;
        if(olr == olrForbidden) col = 0x804000;
  
        if(col)
          displaystr(vid.xres/2, vid.yres - vid.fsize*4, 2, vid.fsize, XLAT(olrDescriptions[olr], cwt.c->land, tr, treasureType(cwt.c->land)), col, 8);
  
        }
      }
    dialog::displayPageButtons(3, 0);
    mouseovers = "";
    keyhandler = [] (int sym, int uni) {
      if(plain) dialog::handleNavigation(sym, uni);
      
      if(orbmap.count(uni)) {
        eItem orb = orbmap[uni];
        if(!remaining[orb]) ;
        else if(orb == itOrbMirror) {
          mirroring = !mirroring;
          // an amusing message
          if(remaining[itOrbMirror] >= 2 && !mirroring) 
            addMessage(XLAT("You mirror %the1.", orb));
          if(mirroring) {
            bool next = false;
            forCellEx(c2, cwt.c) if(c2->wall == waMirror || c2->wall == waCloud || c2->wall == waMirrorWall)
              next = true;
            if(!next) {
              addMessage("You need to stand next to a magic mirror or cloud to use %the1.", itOrbMirror);
              mirroring = false;
              }
            }
          }
        else if(mirroring) {
          if(usedup[orb] >= TESTMIRRORED) {
            addMessage("Each orb type can be mirrored only once.");
            mirroring = false;
            }
          else if(remaining[orb]) {
            usedup[itOrbMirror]++;
            usedup[orb] += MIRRORED;
            usedup[orb] -= mirrorqty(orb);
            usedup[itGreenStone]--;
            addMessage(XLAT("You mirror %the1.", orb));
            mirroring = false;
            }
          else mirroring = false;
          }
        else if((isHaunted(cwt.c->land) || cwt.c->land == laDungeon) && orb == itOrbSafety) {
          addMessage(XLAT("This would only move you deeper into the trap!"));
          }
        else {
          eItem it = cwt.c->item; 
          cwt.c->item = orbmap[uni];
          collectItem(cwt.c, true);
          if(!cwt.c->item) usedup[orbmap[uni]]++;
          if(getOLR(it, getPrizeLand()))
            usedForbidden = true;
          cwt.c->item = it;
          checkmove();
          popScreenAll();
          }
        }
      
      else if(uni == '1') plain = !plain;
      else if(sym == SDLK_F1) 
        gotoHelp(which ? generateHelpForItem(which) : NODESCYET);
      else if(doexiton(sym, uni)) popScreen();
      };
    }

  void applyBox(eItem it) {
    applyBoxNum(usedup[it]);
    }
  
  int incheck;
  
  void check(int delta) {
    incheck += delta;
    for(int i=0; i<ittypes; i++) {
      eItem it = eItem(i);
      if(itemclass(it) == IC_ORB)
        items[it] += delta * remaining[it] * orbcharges(it);
      }
    }

  }

  