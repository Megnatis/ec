	/****  current default parameters initialization  ****/

#ifndef NEW_HEADER
#include <gen_dpi.c>
#endif

#ifdef GHOUDS_NIROU
#include <ghds_dpi.c>
#endif

#ifdef DARAI_KERMAN
#include <dark_dpi.c>
#endif

#ifdef BELAIR_CO
#include <belr_dpi.c>
#endif

#ifdef MASHAD_JAIL
#include <mdjl_dpi.c>
#endif

#ifdef SHESHEA_MINA
#include <mina_dpi.c>
#endif

#ifdef BANK_SEPAH0_MASHAD
#include <msb0_dpi.c>
#endif

#ifdef IRAN_SYSTEM
#include <irsy_dpi.c>
#endif

#ifdef MASHAD_RAILWAY
#include <mdrl_dpi.c>
#endif

#ifdef BARG_MANTAGEH_32_1
#include <b321_dpi.c>
#endif

#ifdef BARG_MANTAGEH_48_1
#include <b481_dpi.c>
#endif

#ifdef BANK_SEPAH_KARAJ
#include <bskj_dpi.c>
#endif

#ifdef AH_RAILWAY
#include <ah_dpi.c>
#endif

#ifdef GM_RAILWAY
#include <gm_dpi.c>
#endif

#ifdef ARAK1_RAILWAY
#include <ak_dpi.c>
#endif

#ifdef BAYAZ_RAILWAY
#include <bz_dpi.c>
#endif

#ifdef SOROOSH
#include <srsh_dpi.c>
#endif

#ifdef KISH_HOTEL
#include <kish_dpi.c>
#endif

#ifdef IRAN_VANET
#include <iran_dpi.c>
#endif

#ifdef INDUSTRIAL_BUILDING_INC
#include <skhs_dpi.c>
#endif

#ifdef TRANSIT_TEST
#include <tr_dpi.c>
#endif

#ifdef KARAJ_RAILWAY
#include <kr_dpi.c>
#endif

#ifdef TANGE_HAFT
#include <th_dpi.c>
#endif

#ifdef GARMSAR
#include <gr_dpi.c>
#endif

#ifdef TAAVONI
#include <ta_dpi.c>
#endif

#ifdef BA_RAILWAY
#include <ba_dpi.c>
#endif

#ifdef KM_RAILWAY
#include <km_dpi.c>
#endif

#ifdef SJ_RAILWAY
#include <sj_dpi.c>
#endif

#ifdef BG_RAILWAY
#include <bg_dpi.c>
#endif

#ifdef SA_RAILWAY
#include <sa_dpi.c>
#endif

#ifdef SS_RAILWAY
#include <ss_dpi.c>
#endif

#ifdef TCI_DPI
#include <tci_dpi.c>
#endif

#ifdef TCI_DPI_60
#include <tc60_dpi.c>
#endif

#ifdef TCI_DPI_90
#include <tc90_dpi.c>
#endif

#ifdef STD_DPI_60	// not used anymore
#include <ST60_dpi.c>
#endif

#if defined TCI_KORDLAR
#include <tcikrdpi.c>
#endif

#ifdef HOZE_ELMIE
#include <hoze_dpi.c>
#endif

#ifdef AAFTAAB_GARDAAN
#include <afta_dpi.c>
#endif

#ifdef TCI_SH
#include <tcishdpi.c>
#endif

#ifdef TCI_HORAND
#include <tchrdpi.c>
#endif

#ifdef NAFT
#include <naft_dpi.c>
#endif

#ifdef GHAND_SHIRIN
#include <tci_dpi.c>
#endif

#ifdef SARAKHS_RAILWAY
#include <sr_dpi.c>
#endif

#ifdef ARDAG
#include <ardg_dpi.c>
#endif

#if (NO_CO != 0)
	for(i=0;i < NO_CO;i++){   // to be compatible with older versions
		ctai[i] &= ~0xf000;
		ctai[i] |= (ctai[i]&0x0007) << 12;
		ctai[i] &= ~0x0007;
	}
#endif
