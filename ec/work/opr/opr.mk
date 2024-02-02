
OBJ = obj1/omain.O      \
      obj1/oprtn.O      \
      obj1/oprmenu.O    \
      obj1/pdul.O       \
      obj1/db.O         \
      obj1/onet.O       \
      obj1/wf.O         \
      obj1/params.O     \
      obj1/cpf.O        \
      obj1/depm.O       \
      obj1/deprtn.O     \
      obj1/ep.O         \
      obj1/filevar1.O   \
      obj1/filevar2.O   \
      obj1/filevar3.O   \
      obj1/filevar5.O   \
      obj1/dep_parm.O   \
      obj1/lapb.O       \
      obj1/sml.O        \
      obj1/smllyr1.O    \
      obj1/glb_rtns.O   \
      obj1/mnul_tst.O   \
      obj1/gcc_func.O   \
      obj1/dpi_file.O   \
      obj1/btch_ep.O

opr.exe : $(OBJ)
        gcc $(OBJ) -s -Wl,-M -o opr.exe


