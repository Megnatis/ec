obj1\omain.o: omtfp.h opr_net.h wg.h ov.h ovpc.h omain.c
        c2ogc1 omain c:\ec\work\opr\obj1

obj1\oprtn.o: omtfp.h opr_net.h oprtn.c
        c2ogc1 oprtn c:\ec\work\opr\obj1

obj1\oprmenu.o: omtfp.h oprmenu.c
        c2ogc1 oprmenu c:\ec\work\opr\obj1

obj1\pdul.o: omtfp.h pdul.c
        c2ogc1 pdul c:\ec\work\opr\obj1

obj1\db.o: omtfp.h db.c
        c2ogc1 db c:\ec\work\opr\obj1

obj1\onet.o: omtfp.h opr_net.h onet.c
        c2ogc1 onet c:\ec\work\opr\obj1

obj1\wf.o: wg.h wv.h wf.c
        c2ogc1 wf c:\ec\work\opr\obj1

obj1\params.o: params.c
        c2ogc1 params c:\ec\work\opr\obj1

obj1\cpf.o: omtfp.h opr_net.h c:\ec\work\sys\emmacro.h cpf.c
        c2ogc1 cpf c:\ec\work\opr\obj1

obj1\depm.o: omtfp.h wg.h depmtfp.h depv.h depm.c
        c2ogc1 depm c:\ec\work\opr\obj1

obj1\deprtn.o: omtfp.h opr_net.h wg.h depmtfp.h deprtn.c
        c2ogc1 deprtn c:\ec\work\opr\obj1

obj1\ep.o: omtfp.h wg.h depmtfp.h c:\ec\work\sys\lapb.var c:\ec\work\sys\sml.var ep.c
        c2ogc1 ep c:\ec\work\opr\obj1

obj1\filevar1.o: omtfp.h wg.h depmtfp.h filevar1.c
        c2ogc1 filevar1 c:\ec\work\opr\obj1

obj1\filevar2.o: omtfp.h wg.h depmtfp.h filevar2.c
        c2ogc1 filevar2 c:\ec\work\opr\obj1

obj1\filevar3.o: omtfp.h filevar3.c
        c2ogc1 filevar3 c:\ec\work\opr\obj1

obj1\filevar5.o: omtfp.h filevar5.c
        c2ogc1 filevar5 c:\ec\work\opr\obj1

obj1\dep_parm.o: dep_parm.c
        c2ogc1 dep_parm c:\ec\work\opr\obj1

obj1\lapb.o: omtfp.h c:\ec\work\sys\lapb.c
        c2ogc2 lapb c:\ec\work\opr\obj1

obj1\sml.o: omtfp.h opr_net.h c:\ec\work\sys\sml.c
        c2ogc2 sml c:\ec\work\opr\obj1

obj1\smllyr1.o: omtfp.h  c:\ec\work\sys\smllyr1.c
        c2ogc2 smllyr1 c:\ec\work\opr\obj1

obj1\glb_rtns.o: omtfp.h  c:\ec\work\sys\glb_rtns.c
        c2ogc2 glb_rtns c:\ec\work\opr\obj1

obj1\mnul_tst.o: omtfp.h wg.h depmtfp.h opr_net.h mnul_tst.c
        c2ogc1 mnul_tst c:\ec\work\opr\obj1

obj1\gcc_func.o: omtfp.h gcc_func.c
        c2ogc1 gcc_func c:\ec\work\opr\obj1

obj1\dpi_file.o: omtfp.h dpi_file.c
        c2ogc1 dpi_file c:\ec\work\opr\obj1

obj1\btch_ep.o: omtfp.h wg.h depmtfp.h  btch_ep.c
        c2ogc1 btch_ep c:\ec\work\opr\obj1

opr.exe :
        redir -e obj1/opr.map -o p1 djmake -f opr.mk


