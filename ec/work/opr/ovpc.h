
BYTE lcdbufptr;

int VSG;  /* video segment address */
BYTE scrnbuf[25][80];
BYTE scrnbuf1[70][50][80];      // enough space for 70000 suscribers, currently sub_no in pcpmftx is limited to 65536
//BYTE attr[25][80];
CHAT_TYPE attr[25][80];
BYTE savescrn[25][80];

#if ((defined OPR_EM_CONNECTION) && (defined EM_CONT_SEP_OPR_Q) )
BYTE *oplbuf=(BYTE *) "\
浜様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様融\
� KIATEL         敖陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳朕                  �\
�                � 1234567890123456789012345678901234567890 �                  �\
�                � 1234567890123456789012345678901234567890 �                  �\
�                青陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳潰                  �\
�    敖陳堕陳�          敖朕      敖陳堕陳�     敖朕              敖陳堕陳�    �\
�    � S � L �          外鯵      � S � L �     外鯵              � S � L �    �\
�    青陳祖陳�          青潰      青陳祖陳�     青潰              青陳祖陳�    �\
�                      Source                  Destination                     �\
麺様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様郵\
�                             敖陳朕                                           �\
菜陳陳堕陳賃陳陳堕陳賃陳陳�   �    �                 敖陳賃陳陳堕陳賃陳陳堕陳朕�\
些C.O.�C.O.�C.O.�C.O.�C.O.�   �    �  敖陳堕陳堕陳�  �E&M �RCL �EXTE�INTE�ATT 該\
些GRP1�GRP2�GRP3�GRP4�GRP5�   団陳調  � 1 � 2 � 3 �  �QUE �    �RNAL�RNAL�RLS 該\
裟陳陳祖陳珍陳陳祖陳珍陳陳�   �    �  団陳田陳田陳�  青陳珍陳陳祖陳珍陳陳祖陳潰�\
菜陳陳堕陳賃陳陳堕陳賃陳陳�   �    �  � 4 � 5 � 6 �  敖陳賃陳陳堕陳賃陳陳堕陳朕�\
些E&M �E&M �E&M �E&M �E&M �   団陳調  団陳田陳田陳�  �E&M �SRC �DEST�OVER�TONE該\
些GRP1�GRP2�GRP3�GRP4�GRP5�   �    �  � 7 � 8 � 9 �  �GRP6�    �    �RIDE�RING該\
裟陳陳祖陳珍陳陳祖陳珍陳陳�   �    �  団陳田陳田陳�  青陳珍陳陳祖陳珍陳陳祖陳潰�\
菜陳陳堕陳賃陳陳堕陳賃陳陳�   団陳調  � * � 0 � # �  敖陳賃陳陳堕陳賃陳陳堕陳朕�\
些NITE�PROG�OPR �FRC.�SRL �   �RLS �  青陳祖陳祖陳�  �HOLD�HOLD�HOLD�HOLD�PARK該\
些    �    �HOOK�RLS �XFER�   �    �                 � 1  � 2  � 3  � 4  �    該\
裟陳陳祖陳珍陳陳祖陳珍陳陳�   青陳潰                 青陳珍陳陳祖陳珍陳陳祖陳潰�\
�                                                                              �\
藩様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様夕\
";
#ifdef OPERATOR_FARSI
BYTE *foplbuf=(BYTE *) "\
浜様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様融\
�                敖陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳朕       ����� �遒� �\
�                � 1234567890123456789012345678901234567890 �                  �\
�                � 1234567890123456789012345678901234567890 �                  �\
�                青陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳潰                  �\
�    敖陳堕陳�          敖朕      敖陳堕陳�     敖朕              敖陳堕陳�    �\
�    � S � L �          外鯵      � S � L �     外鯵              � S � L �    �\
�    青陳祖陳�          青潰      青陳祖陳�     青潰              青陳祖陳�    �\
�                      �����                    ↑跿                           �\
麺様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様郵\
�                             敖陳朕                                           �\
菜陳陳堕陳賃陳陳堕陳賃陳陳�   �    �                 敖陳賃陳陳堕陳賃陳陳堕陳朕�\
些����栢���栢���栢���栢����   �    �  敖陳堕陳堕陳�  劾��ヽ-�� 劾��ヽ���ヽATT 該\
些� � �� � �� � �� � �� � �   団陳調  � � � � � � �  �E&M ���� ����ヽ顱���RLS 該\
裟陳陳祖陳珍陳陳祖陳珍陳陳�   �    �  団陳田陳田陳�  青陳珍陳陳祖陳珍陳陳祖陳潰�\
菜陳陳堕陳賃陳陳堕陳賃陳陳�   �    �  � � � � � � �  敖陳賃陳陳堕陳賃陳陳堕陳朕�\
些E&M �E&M �E&M �E&M �E&M �   団陳調  団陳田陳田陳�  �E&M �����晦�跿�OVER�TONE該\
些� � �� � �� � �� � �� � �   �    �  � � � � � � �  �� � �    �    �RIDE�RING該\
裟陳陳祖陳珍陳陳祖陳珍陳陳�   �    �  団陳田陳田陳�  青陳珍陳陳祖陳珍陳陳祖陳潰�\
菜陳陳堕陳賃陳陳堕陳賃陳陳�   団陳調  � * � � � # �  敖陳賃陳陳堕陳賃陳陳堕陳朕�\
些◯齊������OPR �FRC.�SRL �   �RLS �  青陳祖陳祖陳�  � ‰�� ‰�� ‰�� ‰�轄���該\
些 �� � �� �HOOK�RLS �XFER�   �    �                 � �  � �  � �  � �  �    該\
裟陳陳祖陳珍陳陳祖陳珍陳陳�   青陳潰                 青陳珍陳陳祖陳珍陳陳祖陳潰�\
�                                                                              �\
藩様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様夕\
";
#endif
#endif

#if ((defined OPR_EM_CONNECTION) && !(defined EM_CONT_SEP_OPR_Q) )
BYTE *oplbuf=(BYTE *) "\
浜様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様融\
� KIATEL         敖陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳朕                  �\
�                � 1234567890123456789012345678901234567890 �                  �\
�                � 1234567890123456789012345678901234567890 �                  �\
�                青陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳潰                  �\
�    敖陳堕陳�          敖朕      敖陳堕陳�     敖朕              敖陳堕陳�    �\
�    � S � L �          外鯵      � S � L �     外鯵              � S � L �    �\
�    青陳祖陳�          青潰      青陳祖陳�     青潰              青陳祖陳�    �\
�                      Source                  Destination                     �\
麺様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様郵\
�                             敖陳朕                                           �\
菜陳陳堕陳賃陳陳堕陳賃陳陳�   �    �                 敖陳賃陳陳堕陳賃陳陳堕陳朕�\
些C.O.�C.O.�C.O.�C.O.�C.O.�   �    �  敖陳堕陳堕陳�  �TIE �RCL �EXTE�INTE�ATT 該\
些GRP1�GRP2�GRP3�GRP4�GRP5�   団陳調  � 1 � 2 � 3 �  �GRP1�    �RNAL�RNAL�RLS 該\
裟陳陳祖陳珍陳陳祖陳珍陳陳�   �    �  団陳田陳田陳�  青陳珍陳陳祖陳珍陳陳祖陳潰�\
菜陳陳堕陳賃陳陳堕陳賃陳陳�   �    �  � 4 � 5 � 6 �  敖陳賃陳陳堕陳賃陳陳堕陳朕�\
些E&M �E&M �E&M �E&M �E&M �   団陳調  団陳田陳田陳�  �E&M �SRC �DEST�OVER�TONE該\
些GRP1�GRP2�GRP3�GRP4�GRP5�   �    �  � 7 � 8 � 9 �  �GRP6�    �    �RIDE�RING該\
裟陳陳祖陳珍陳陳祖陳珍陳陳�   �    �  団陳田陳田陳�  青陳珍陳陳祖陳珍陳陳祖陳潰�\
菜陳陳堕陳賃陳陳堕陳賃陳陳�   団陳調  � * � 0 � # �  敖陳賃陳陳堕陳賃陳陳堕陳朕�\
些NITE�PROG�OPR �FRC.�SRL �   �RLS �  青陳祖陳祖陳�  �HOLD�HOLD�HOLD�HOLD�PARK該\
些    �    �HOOK�RLS �XFER�   �    �                 � 1  � 2  � 3  � 4  �    該\
裟陳陳祖陳珍陳陳祖陳珍陳陳�   青陳潰                 青陳珍陳陳祖陳珍陳陳祖陳潰�\
�                                                                              �\
藩様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様夕\
";
#ifdef OPERATOR_FARSI
BYTE *foplbuf=(BYTE *) "\
浜様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様融\
�                敖陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳朕       ����� �遒� �\
�                � 1234567890123456789012345678901234567890 �                  �\
�                � 1234567890123456789012345678901234567890 �                  �\
�                青陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳潰                  �\
�    敖陳堕陳�          敖朕      敖陳堕陳�     敖朕              敖陳堕陳�    �\
�    � S � L �          外鯵      � S � L �     外鯵              � S � L �    �\
�    青陳祖陳�          青潰      青陳祖陳�     青潰              青陳祖陳�    �\
�                      �����                    ↑跿                           �\
麺様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様郵\
�                             敖陳朕                                           �\
菜陳陳堕陳賃陳陳堕陳賃陳陳�   �    �                 敖陳賃陳陳堕陳賃陳陳堕陳朕�\
些����栢���栢���栢���栢����   �    �  敖陳堕陳堕陳�  �TIE �-�� 劾��ヽ���ヽATT 該\
些� � �� � �� � �� � �� � �   団陳調  � � � � � � �  �� � ���� ����ヽ顱���RLS 該\
裟陳陳祖陳珍陳陳祖陳珍陳陳�   �    �  団陳田陳田陳�  青陳珍陳陳祖陳珍陳陳祖陳潰�\
菜陳陳堕陳賃陳陳堕陳賃陳陳�   �    �  � � � � � � �  敖陳賃陳陳堕陳賃陳陳堕陳朕�\
些E&M �E&M �E&M �E&M �E&M �   団陳調  団陳田陳田陳�  �E&M �����晦�跿�OVER�TONE該\
些� � �� � �� � �� � �� � �   �    �  � � � � � � �  �� � �    �    �RIDE�RING該\
裟陳陳祖陳珍陳陳祖陳珍陳陳�   �    �  団陳田陳田陳�  青陳珍陳陳祖陳珍陳陳祖陳潰�\
菜陳陳堕陳賃陳陳堕陳賃陳陳�   団陳調  � * � � � # �  敖陳賃陳陳堕陳賃陳陳堕陳朕�\
些◯齊������OPR �FRC.�SRL �   �RLS �  青陳祖陳祖陳�  � ‰�� ‰�� ‰�� ‰�轄���該\
些 �� � �� �HOOK�RLS �XFER�   �    �                 � �  � �  � �  � �  �    該\
裟陳陳祖陳珍陳陳祖陳珍陳陳�   青陳潰                 青陳珍陳陳祖陳珍陳陳祖陳潰�\
�                                                                              �\
藩様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様夕\
";
#endif
#endif
#if (defined OPR_PRI_CONNECTION)
#ifdef OPR_SIP_CONNECTION
BYTE *oplbuf=(BYTE *) "\
浜様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様融\
� KIATEL         敖陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳朕                  �\
�                � 1234567890123456789012345678901234567890 �                  �\
�                � 1234567890123456789012345678901234567890 �                  �\
�                青陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳潰                  �\
�    敖陳堕陳�          敖朕      敖陳堕陳�     敖朕              敖陳堕陳�    �\
�    � S � L �          外鯵      � S � L �     外鯵              � S � L �    �\
�    青陳祖陳�          青潰      青陳祖陳�     青潰              青陳祖陳�    �\
�                      Source                  Destination                     �\
麺様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様郵\
�                             敖陳朕                                           �\
菜陳陳堕陳賃陳陳堕陳賃陳陳�   �    �                 敖陳賃陳陳堕陳賃陳陳堕陳朕�\
些C.O.�C.O.�C.O.�C.O.�C.O.�   �    �  敖陳堕陳堕陳�  �C.O.�RCL �EXTE�INTE�ATT 該\
些GRP1�GRP2�GRP3�GRP4�GRP5�   団陳調  � 1 � 2 � 3 �  �GRP6�    �RNAL�RNAL�RLS 該\
裟陳陳祖陳珍陳陳祖陳珍陳陳�   �    �  団陳田陳田陳�  青陳珍陳陳祖陳珍陳陳祖陳潰�\
菜陳陳堕陳賃陳陳堕陳賃陳陳�   �    �  � 4 � 5 � 6 �  敖陳賃陳陳堕陳賃陳陳堕陳朕�\
些TIE �TIE �TIE �TIE �SIP �   団陳調  団陳田陳田陳�  �PRI.�SRC �DEST�OVER�TONE該\
些GRP1�GRP2�GRP3�GRP4�REQ �   �    �  � 7 � 8 � 9 �  �GRP �    �    �RIDE�RING該\
裟陳陳祖陳珍陳陳祖陳珍陳陳�   �    �  団陳田陳田陳�  青陳珍陳陳祖陳珍陳陳祖陳潰�\
菜陳陳堕陳賃陳陳堕陳賃陳陳�   団陳調  � * � 0 � # �  敖陳賃陳陳堕陳賃陳陳堕陳朕�\
些NITE�PROG�OPR �FRC.�SRL �   �RLS �  青陳祖陳祖陳�  �HOLD�HOLD�HOLD�HOLD�PARK該\
些    �    �HOOK�RLS �XFER�   �    �                 � 1  � 2  � 3  � 4  �    該\
裟陳陳祖陳珍陳陳祖陳珍陳陳�   青陳潰                 青陳珍陳陳祖陳珍陳陳祖陳潰�\
�                                                                              �\
藩様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様夕\
";
#ifdef OPERATOR_FARSI
BYTE *foplbuf=(BYTE *) "\
浜様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様融\
�                敖陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳朕       ����� �遒� �\
�                � 1234567890123456789012345678901234567890 �                  �\
�                � 1234567890123456789012345678901234567890 �                  �\
�                青陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳潰                  �\
�    敖陳堕陳�          敖朕      敖陳堕陳�     敖朕              敖陳堕陳�    �\
�    � S � L �          外鯵      � S � L �     外鯵              � S � L �    �\
�    青陳祖陳�          青潰      青陳祖陳�     青潰              青陳祖陳�    �\
�                      �����                    ↑跿                           �\
麺様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様郵\
�                             敖陳朕                                           �\
菜陳陳堕陳賃陳陳堕陳賃陳陳�   �    �                 敖陳賃陳陳堕陳賃陳陳堕陳朕�\
些����栢���栢���栢���栢����   �    �  敖陳堕陳堕陳�  栢����-�� 劾��ヽ���ヽATT 該\
些� � �� � �� � �� � �� � �   団陳調  � � � � � � �  �� � ���� ����ヽ顱���RLS 該\
裟陳陳祖陳珍陳陳祖陳珍陳陳�   �    �  団陳田陳田陳�  青陳珍陳陳祖陳珍陳陳祖陳潰�\
菜陳陳堕陳賃陳陳堕陳賃陳陳�   �    �  � � � � � � �  敖陳賃陳陳堕陳賃陳陳堕陳朕�\
些TIE �TIE �TIE �TIE �SIP �   団陳調  団陳田陳田陳�  �PRI.�����晦�跿�OVER�TONE該\
些� � �� � �� � �� � �  � �   �    �  � � � � � � �  �  � �    �    �RIDE�RING該\
裟陳陳祖陳珍陳陳祖陳珍陳陳�   �    �  団陳田陳田陳�  青陳珍陳陳祖陳珍陳陳祖陳潰�\
菜陳陳堕陳賃陳陳堕陳賃陳陳�   団陳調  � * � � � # �  敖陳賃陳陳堕陳賃陳陳堕陳朕�\
些◯齊������OPR �FRC.�SRL �   �RLS �  青陳祖陳祖陳�  � ‰�� ‰�� ‰�� ‰�轄���該\
些 �� � �� �HOOK�RLS �XFER�   �    �                 � �  � �  � �  � �  �    該\
裟陳陳祖陳珍陳陳祖陳珍陳陳�   青陳潰                 青陳珍陳陳祖陳珍陳陳祖陳潰�\
�                                                                              �\
藩様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様夕\
";
#endif
#else //OPR_SIP_CONNECTION
BYTE *oplbuf=(BYTE *) "\
浜様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様融\
� KIATEL         敖陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳朕                  �\
�                � 1234567890123456789012345678901234567890 �                  �\
�                � 1234567890123456789012345678901234567890 �                  �\
�                青陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳潰                  �\
�    敖陳堕陳�          敖朕      敖陳堕陳�     敖朕              敖陳堕陳�    �\
�    � S � L �          外鯵      � S � L �     外鯵              � S � L �    �\
�    青陳祖陳�          青潰      青陳祖陳�     青潰              青陳祖陳�    �\
�                      Source                  Destination                     �\
麺様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様郵\
�                             敖陳朕                                           �\
菜陳陳堕陳賃陳陳堕陳賃陳陳�   �    �                 敖陳賃陳陳堕陳賃陳陳堕陳朕�\
些C.O.�C.O.�C.O.�C.O.�C.O.�   �    �  敖陳堕陳堕陳�  �C.O.�RCL �EXTE�INTE�ATT 該\
些GRP1�GRP2�GRP3�GRP4�GRP5�   団陳調  � 1 � 2 � 3 �  �GRP6�    �RNAL�RNAL�RLS 該\
裟陳陳祖陳珍陳陳祖陳珍陳陳�   �    �  団陳田陳田陳�  青陳珍陳陳祖陳珍陳陳祖陳潰�\
菜陳陳堕陳賃陳陳堕陳賃陳陳�   �    �  � 4 � 5 � 6 �  敖陳賃陳陳堕陳賃陳陳堕陳朕�\
些TIE �TIE �TIE �TIE �TIE �   団陳調  団陳田陳田陳�  �PRI.�SRC �DEST�OVER�TONE該\
些GRP1�GRP2�GRP3�GRP4�GRP5�   �    �  � 7 � 8 � 9 �  �GRP �    �    �RIDE�RING該\
裟陳陳祖陳珍陳陳祖陳珍陳陳�   �    �  団陳田陳田陳�  青陳珍陳陳祖陳珍陳陳祖陳潰�\
菜陳陳堕陳賃陳陳堕陳賃陳陳�   団陳調  � * � 0 � # �  敖陳賃陳陳堕陳賃陳陳堕陳朕�\
些NITE�PROG�OPR �FRC.�SRL �   �RLS �  青陳祖陳祖陳�  �HOLD�HOLD�HOLD�HOLD�PARK該\
些    �    �HOOK�RLS �XFER�   �    �                 � 1  � 2  � 3  � 4  �    該\
裟陳陳祖陳珍陳陳祖陳珍陳陳�   青陳潰                 青陳珍陳陳祖陳珍陳陳祖陳潰�\
�                                                                              �\
藩様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様夕\
";
#ifdef OPERATOR_FARSI
BYTE *foplbuf=(BYTE *) "\
浜様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様融\
�                敖陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳朕       ����� �遒� �\
�                � 1234567890123456789012345678901234567890 �                  �\
�                � 1234567890123456789012345678901234567890 �                  �\
�                青陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳潰                  �\
�    敖陳堕陳�          敖朕      敖陳堕陳�     敖朕              敖陳堕陳�    �\
�    � S � L �          外鯵      � S � L �     外鯵              � S � L �    �\
�    青陳祖陳�          青潰      青陳祖陳�     青潰              青陳祖陳�    �\
�                      �����                    ↑跿                           �\
麺様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様郵\
�                             敖陳朕                                           �\
菜陳陳堕陳賃陳陳堕陳賃陳陳�   �    �                 敖陳賃陳陳堕陳賃陳陳堕陳朕�\
些����栢���栢���栢���栢����   �    �  敖陳堕陳堕陳�  栢����-�� 劾��ヽ���ヽATT 該\
些� � �� � �� � �� � �� � �   団陳調  � � � � � � �  �� � ���� ����ヽ顱���RLS 該\
裟陳陳祖陳珍陳陳祖陳珍陳陳�   �    �  団陳田陳田陳�  青陳珍陳陳祖陳珍陳陳祖陳潰�\
菜陳陳堕陳賃陳陳堕陳賃陳陳�   �    �  � � � � � � �  敖陳賃陳陳堕陳賃陳陳堕陳朕�\
些TIE �TIE �TIE �TIE �TIE �   団陳調  団陳田陳田陳�  �PRI.�����晦�跿�OVER�TONE該\
些� � �� � �� � �� � �� � �   �    �  � � � � � � �  �  � �    �    �RIDE�RING該\
裟陳陳祖陳珍陳陳祖陳珍陳陳�   �    �  団陳田陳田陳�  青陳珍陳陳祖陳珍陳陳祖陳潰�\
菜陳陳堕陳賃陳陳堕陳賃陳陳�   団陳調  � * � � � # �  敖陳賃陳陳堕陳賃陳陳堕陳朕�\
些◯齊������OPR �FRC.�SRL �   �RLS �  青陳祖陳祖陳�  � ‰�� ‰�� ‰�� ‰�轄���該\
些 �� � �� �HOOK�RLS �XFER�   �    �                 � �  � �  � �  � �  �    該\
裟陳陳祖陳珍陳陳祖陳珍陳陳�   青陳潰                 青陳珍陳陳祖陳珍陳陳祖陳潰�\
�                                                                              �\
藩様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様夕\
";
#endif //OPR_SIP_CONNECTION
#endif //OPR_PRI_CONNECTION
#endif

#if ((!defined OPR_EM_CONNECTION) && (!defined OPR_PRI_CONNECTION))
BYTE *oplbuf=(BYTE *) "\
浜様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様融\
� KIATEL         敖陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳朕                  �\
�                � 1234567890123456789012345678901234567890 �                  �\
�                � 1234567890123456789012345678901234567890 �                  �\
�                青陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳潰                  �\
�    敖陳堕陳�          敖朕      敖陳堕陳�     敖朕              敖陳堕陳�    �\
�    � S � L �          外鯵      � S � L �     外鯵              � S � L �    �\
�    青陳祖陳�          青潰      青陳祖陳�     青潰              青陳祖陳�    �\
�                      Source                  Destination                     �\
麺様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様郵\
�                             敖陳朕                                           �\
菜陳陳堕陳賃陳陳堕陳賃陳陳�   �    �                 敖陳賃陳陳堕陳賃陳陳堕陳朕�\
些C.O.�C.O.�C.O.�C.O.�C.O.�   �    �  敖陳堕陳堕陳�  �C.O.�RCL �EXTE�INTE�ATT 該\
些GRP1�GRP2�GRP3�GRP4�GRP5�   団陳調  � 1 � 2 � 3 �  �GRP6�    �RNAL�RNAL�RLS 該\
裟陳陳祖陳珍陳陳祖陳珍陳陳�   �    �  団陳田陳田陳�  青陳珍陳陳祖陳珍陳陳祖陳潰�\
菜陳陳堕陳賃陳陳堕陳賃陳陳�   �    �  � 4 � 5 � 6 �  敖陳賃陳陳堕陳賃陳陳堕陳朕�\
些TIE �TIE �TIE �TIE �TIE �   団陳調  団陳田陳田陳�  �D.T.�SRC �DEST�OVER�TONE該\
些GRP1�GRP2�GRP3�GRP4�GRP5�   �    �  � 7 � 8 � 9 �  �GRP1�    �    �RIDE�RING該\
裟陳陳祖陳珍陳陳祖陳珍陳陳�   �    �  団陳田陳田陳�  青陳珍陳陳祖陳珍陳陳祖陳潰�\
菜陳陳堕陳賃陳陳堕陳賃陳陳�   団陳調  � * � 0 � # �  敖陳賃陳陳堕陳賃陳陳堕陳朕�\
些NITE�PROG�OPR �FRC.�SRL �   �RLS �  青陳祖陳祖陳�  �HOLD�HOLD�HOLD�HOLD�PARK該\
些    �    �HOOK�RLS �XFER�   �    �                 � 1  � 2  � 3  � 4  �    該\
裟陳陳祖陳珍陳陳祖陳珍陳陳�   青陳潰                 青陳珍陳陳祖陳珍陳陳祖陳潰�\
�                                                                              �\
藩様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様夕\
";
#ifdef OPERATOR_FARSI
BYTE *foplbuf=(BYTE *) "\
浜様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様融\
�                敖陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳朕       ����� �遒� �\
�                � 1234567890123456789012345678901234567890 �                  �\
�                � 1234567890123456789012345678901234567890 �                  �\
�                青陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳潰                  �\
�    敖陳堕陳�          敖朕      敖陳堕陳�     敖朕              敖陳堕陳�    �\
�    � S � L �          外鯵      � S � L �     外鯵              � S � L �    �\
�    青陳祖陳�          青潰      青陳祖陳�     青潰              青陳祖陳�    �\
�                      �����                    ↑跿                           �\
麺様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様郵\
�                             敖陳朕                                           �\
菜陳陳堕陳賃陳陳堕陳賃陳陳�   �    �                 敖陳賃陳陳堕陳賃陳陳堕陳朕�\
些����栢���栢���栢���栢����   �    �  敖陳堕陳堕陳�  栢����-�� 劾��ヽ���ヽATT 該\
些� � �� � �� � �� � �� � �   団陳調  � � � � � � �  �� � ���� ����ヽ顱���RLS 該\
裟陳陳祖陳珍陳陳祖陳珍陳陳�   �    �  団陳田陳田陳�  青陳珍陳陳祖陳珍陳陳祖陳潰�\
菜陳陳堕陳賃陳陳堕陳賃陳陳�   �    �  � � � � � � �  敖陳賃陳陳堕陳賃陳陳堕陳朕�\
些TIE �TIE �TIE �TIE �TIE �   団陳調  団陳田陳田陳�  �D.T.�����晦�跿�OVER�TONE該\
些� � �� � �� � �� � �� � �   �    �  � � � � � � �  �� � �    �    �RIDE�RING該\
裟陳陳祖陳珍陳陳祖陳珍陳陳�   �    �  団陳田陳田陳�  青陳珍陳陳祖陳珍陳陳祖陳潰�\
菜陳陳堕陳賃陳陳堕陳賃陳陳�   団陳調  � * � � � # �  敖陳賃陳陳堕陳賃陳陳堕陳朕�\
些◯齊������OPR �FRC.�SRL �   �RLS �  青陳祖陳祖陳�  � ‰�� ‰�� ‰�� ‰�轄���該\
些 �� � �� �HOOK�RLS �XFER�   �    �                 � �  � �  � �  � �  �    該\
裟陳陳祖陳珍陳陳祖陳珍陳陳�   青陳潰                 青陳珍陳陳祖陳珍陳陳祖陳潰�\
�                                                                              �\
藩様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様夕\
";
#endif
#endif

BYTE chbuf[4 * 1000 +1];
BYTE *chlbuf=&chbuf[0];

#ifdef OPERATOR_FARSI
BYTE fchbuf[4 * 1000 +1];
BYTE *fchlbuf=&fchbuf[0];
#endif

struct LED_POS led_table[MAX_NO_LED]={
        { 2,12, 5,13},{ 7,12,10,13},{12,12,15,13},{17,12,20,13},{22,12,25,13},
        { 2,16, 5,17},{ 7,16,10,17},{12,16,15,17},{17,16,20,17},{22,16,25,17},
        { 2,20, 5,21},{ 7,20,10,21},{12,20,15,21},{17,20,20,21},{22,20,25,21},
        {54,12,57,13},{59,12,62,13},{64,12,67,13},{69,12,72,13},{74,12,77,13},
        {54,16,57,17},{59,16,62,17},{64,16,67,17},{69,16,72,17},{74,16,77,17},
        {54,20,57,21},{59,20,62,21},{64,20,67,21},{69,20,72,21},{74,20,77,21},
/*
        {31,11,34,12},
        {31,14,34,15},
*/
        {6,6,8,6},
        {10,6,12,6},

        {31,17,34,18},
        {31,20,34,21},
        {25, 6,26, 6},
        {49, 6,50, 6},

        {35,6,37,6},{39,6,41,6},
        {67,6,69,6},{71,6,73,6}
};

#ifdef NEW_DEP
struct LED_POS dep_led_table[MAX_NO_LED]={
        {80,25,80,25},{80,25,80,25},{80,25,80,25},{80,25,80,25},{80,25,80,25},
        {80,25,80,25},{80,25,80,25},{80,25,80,25},{80,25,80,25},{80,25,80,25},
        {80,25,80,25},{80,25,80,25},{80,25,80,25},{80,25,80,25},{80,25,80,25},
        {59,22,60,22},{50,22,51,22},{29,22,30,22},{20,22,21,22},{80,25,80,25},
        {80,25,80,25},{80,25,80,25},{80,25,80,25},{80,25,80,25},{80,25,80,25},
        {80,25,80,25},{80,25,80,25},{80,25,80,25},{80,25,80,25},{80,25,80,25},
/*
        {31,11,34,12},
        {31,14,34,15},
*/
        {6,22,8,22},
        {10,22,12,22},

        {80,25,80,25},
        {80,25,80,25},
        {80,25,80,25},
        {80,25,80,25},

        {37,22,39,22},{41,22,43,22},
        {69,22,71,22},{73,22,75,22}
};
#endif
BYTE key_code[256]={
/*-1,-1,-1,-1,-1,-1,-1,-1,-1,46, -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,*/    /*  0-19 */
/* CR also returns DISP_CAS1_STATUS */
-1,-1,-1,-1,-1,-1,-1,-1,-1,46, -1,-1,-1,51,-1,-1,-1,-1,-1,-1,    /*  0-19 */
/*-1,-1,-1,-1,-1,-1,-1,53,-1,-1, -1,-1,33,-1,-1,-1,-1,-1,-1,24,*/    /* 20-39 */
-1,-1,-1,-1,-1,-1,-1,53,-1,-1, -1,-1,33,-1,-1,45,-1,-1,-1,24,    /* 20-39 */
-1,-1,43,-1,27,45,28,29,44,34, 35,36,37,38,39,40,41,42,-1,23,    /* 40-59 */
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    /* 60-79 */
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, -1,18,-1,19,-1,-1,-1, 5,14,12,    /* 80-99 */
 7, 2, 8, 9,32,15,20,21,22,26, 25,16,17, 0, 3, 6, 4,31,13, 1,    /*100-119*/
11,30,10,-1,-1,47,-1,-1,                                         /*120-127*/

                                                                 // 128+
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    /*  0-19 */
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    /* 20-39 */
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,-1,48,    /* 40-59 */
49,59,58,55,54,52,51,57,56,-1, -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    /* 60-79 */
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    /* 80-99 */
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    /*100-119*/
-1,-1,-1,-1,-1,-1,-1,-1                                          /*120-127*/
};
/* TAB=9 is used for toggling operator language between English & Farsi */
/* 46 is offset of TOG_LANGUAGE event in kctt[] */
/* }=0x7d is used for shell to DOS prompt, 47 is offset of SHELL event */
/* in kctt[] */
/* F1-F12 are returned as extended ASCII code (0x3b-0x46,59-70), we make
   get_c return 14-25 for that,
   then use it again for new stimulus such as EM_GRP7-10, or SHELL(47)
*/
// 17/7/78: get_c changed to return extended codes in range of 128-255,
// so F1-F12 are now returned as 0x80+(0x3b..0x46) (59-70)

// code 55 (from kctt[]) is DISP_TRUNK_STATUS, allocated for F5

BYTE key_table[25][80]={    /* key-codes 0-45 are used */
/*       0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 */
/* 0 */ {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
/* 1 */ {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
/* 2 */ {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
/* 3 */ {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
/* 4 */ {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
/* 5 */ {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
/* 6 */ {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
/* 7 */ {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
/* 8 */ {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
/* 9 */ {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
/*10 */ {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
/*11 */ {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,30,30,30,30,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
/*12 */ {-1,-1, 0, 0, 0, 0,-1, 1, 1, 1, 1,-1, 2, 2, 2, 2,-1, 3, 3, 3, 3,-1, 4, 4, 4, 4,-1,-1,-1,-1,-1,30,30,30,30,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,15,15,15,15,-1,16,16,16,16,-1,17,17,17,17,-1,18,18,18,18,-1,19,19,19,19,-1,-1},
/*13 */ {-1,-1, 0, 0, 0, 0,-1, 1, 1, 1, 1,-1, 2, 2, 2, 2,-1, 3, 3, 3, 3,-1, 4, 4, 4, 4,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,34,34,34,-1,35,35,35,-1,36,36,36,-1,-1,-1,-1,15,15,15,15,-1,16,16,16,16,-1,17,17,17,17,-1,18,18,18,18,-1,19,19,19,19,-1,-1},
/*14 */ {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,31,31,31,31,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
/*15 */ {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,31,31,31,31,-1,-1,-1,-1,37,37,37,-1,38,38,38,-1,39,39,39,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
/*16 */ {-1,-1, 5, 5, 5, 5,-1, 6, 6, 6, 6,-1, 7, 7, 7, 7,-1, 8, 8, 8, 8,-1, 9, 9, 9, 9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,20,20,20,20,-1,21,21,21,21,-1,22,22,22,22,-1,23,23,23,23,-1,24,24,24,24,-1,-1},
/*17 */ {-1,-1, 5, 5, 5, 5,-1, 6, 6, 6, 6,-1, 7, 7, 7, 7,-1, 8, 8, 8, 8,-1, 9, 9, 9, 9,-1,-1,-1,-1,-1,32,32,32,32,-1,-1,-1,-1,40,40,40,-1,41,41,41,-1,42,42,42,-1,-1,-1,-1,20,20,20,20,-1,21,21,21,21,-1,22,22,22,22,-1,23,23,23,23,-1,24,24,24,24,-1,-1},
/*18 */ {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,32,32,32,32,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
/*19 */ {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,43,43,43,-1,44,44,44,-1,45,45,45,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
/*20 */ {-1,-1,10,10,10,10,-1,11,11,11,11,-1,12,12,12,12,-1,13,13,13,13,-1,14,14,14,14,-1,-1,-1,-1,-1,33,33,33,33,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,25,25,25,25,-1,26,26,26,26,-1,27,27,27,27,-1,28,28,28,28,-1,29,29,29,29,-1,-1},
/*21 */ {-1,-1,10,10,10,10,-1,11,11,11,11,-1,12,12,12,12,-1,13,13,13,13,-1,14,14,14,14,-1,-1,-1,-1,-1,33,33,33,33,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,25,25,25,25,-1,26,26,26,26,-1,27,27,27,27,-1,28,28,28,28,-1,29,29,29,29,-1,-1},
/*22 */ {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
/*23 */ {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
/*24 */ {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
};

BYTE flag;
BYTE lsbs[2]={FREQ1_LSB,FREQ2_LSB};
BYTE msbs[2]={FREQ1_MSB,FREQ2_MSB};

DATTIM sdt;  /* system date time */
DATTIM billing_sdt;  /* system date time */

#ifdef DAY_NIGHT_PARAMETERS_FILE
char *pfna[MAX_NO_OPR_CARD][NO_LOAD_PARM_FILE]={
       {"dayparm1.dat",
        "nitparm1.dat"},

       {"dayparm2.dat",
        "nitparm2.dat"},

       {"dayparm3.dat",
        "nitparm3.dat"},
        };  /* parameters file name array */
char *pfna_bak[MAX_NO_OPR_CARD][NO_LOAD_PARM_FILE]={
       {"dayparm1.bak",
        "nitparm1.bak"},

       {"dayparm2.bak",
        "nitparm2.bak"},

       {"dayparm3.bak",
        "nitparm3.bak"},
        };  /* parameters file name array */
char *pfna_bak2[MAX_NO_OPR_CARD][NO_LOAD_PARM_FILE]={
       {".p1d",
        ".p1n"},

       {".p2d",
        ".p2n"},

       {".p3d",
        ".p3n"},
        };  /* parameters file name array */
#else
char *pfna[MAX_NO_OPR_CARD][1]={
       {"parm1.dat"},
       {"parm2.dat"},
       {"parm3.dat"}
        };  /* parameters file name array */
char *pfna_bak[MAX_NO_OPR_CARD][1]={
       {"parm1.bak"},
       {"parm2.bak"},
       {"parm3.bak"}
        };  /* parameters file name array */
char *pfna_bak2[MAX_NO_OPR_CARD][1]={
       {".p1"},
       {".p2"},
       {".p3"}
        };  /* parameters file name array */
#endif



#ifdef PARAMETER_DOWN_UP_LOAD

FILE *pfp[MAX_NO_OPR_CARD];  /* parameters' file pointer */
// CPULOAD
//unsigned int buf[READ_BLOCK_SIZE];
unsigned short buf[READ_BLOCK_SIZE];

#ifndef LIM
#define MAX_NO_LOAD_PARM        NO_LOAD_PARM
#else
// in LIM, NO_LOAD_PARM of limb or limc can be greater than that of lima
// hence we should reserve space for a maximum number of NO_LOAD_PARM
// between various lims
#define MAX_NO_LOAD_PARM        3000    // maximum 6000 bytes file size
                                        // rlwysa parm1.dat is 4356 bytes
#endif

//unsigned int parmb[MAX_NO_LOAD_PARM];  /* parameters' buffer */
unsigned short parmb[MAX_NO_LOAD_PARM];  /* parameters' buffer */
int pbs;  /* parameters' buffer size */

int no_load_parm[MAX_NO_OPR_CARD];
int valid_parm_file_size[MAX_NO_OPR_CARD];

#endif

unsigned int card;
unsigned int base_addr[7];

#ifdef DISPLAY_STATUS_ACT
BYTE dxsf;
#endif

#ifdef DISPLAY_EM_STATUS
BYTE dtsf;
#endif

#ifdef DISPLAY_CAS1_STATUS
BYTE d1sf;
#endif

//EM_RECHECK start

#if ((defined DISPLAY_EM_STATUS) || (defined DISPLAY_CAS1_STATUS))
/*
#ifdef LIM
char *emstate_name[NO_EM_STATES+NO_CAS3_STATES]={
#else
char *emstate_name[NO_EM_STATES]={
#endif
*/
char *emstate_name[EMSTATE_NAME_ARR_SIZE]={

"Wait for Ack     ",    //      AWAIT_ACK
"Ack Received     ",    //      ACK
"OGT is Block     ",    //      OGT_BLOCK
"ICT is Block     ",    //      ICT_BLOCK
"Wait for Seize   ",    //      AWAIT_SEIZE
"Gaurd            ",    //      GAURD
"Answer           ",    //      ANSWER
"Wait for Digits  ",    //      AWAIT_DGT
"Wait for SeizeAck",    //      AWAIT_SEIZE_ACK
"ICT Wait for Idle",    //      IC_SGNL_BLOCK
"Congestion       ",    //      CONGESTION
"ICT Talk         ",    //      STD_CONNECT
"Clear Back       ",    //      CLR_BACK
"OGT is Talking   ",    //      TALK
"Metering         ",    //      METERING
"OPR_AWAIT_ACK_EM ",    //      OPR_AWAIT_ACK_EM
"OPR_ACK_EM       ",    //      OPR_ACK_EM
"OPR_ANSWER_EM    ",    //      OPR_ANSWER_EM
"OPR_TALK_EM      ",    //      OPR_TALK_EM
"OPR_METERING_EM  ",    //      OPR_METERING_EM
"Flash on OGT     ",    //      FLASH_DTCT_OGT_EM_PULSE
"Flash on ICT     ",    //      FLASH_DTCT_ICT_EM_PULSE
"False Answer     ",    //      EM_FAS
"Init Guard       ",    //      INIT_GAURD
"Local Block      ",    //      LOCAL_BLOCK
"Wait for 1st Dgt ",    //      AWAIT_1ST_DGT_EM
"Input CLF in ACK ",    //      ACK_INPUT_CLF
"Input CLF in Talk",    //      TALK_INPUT_CLF
"ICT Sent Digits  "     //      STD_CONNECT_RINGING
#ifdef RMT_EM
,
"Link Discnnct    ",    //      AWAIT_EM_LINK_DISCONNECT_ACK
"Lcl Blk Disccnct ",    //      AWAIT_EM_LCL_BLK_LINK_DISCONNECT_ACK
"Rmt Blk Discnnct "     //      AWAIT_EM_RMT_BLK_LINK_DISCONNECT_ACK
#endif

//EM_RECHECK end

#ifdef LIM
,
"Wait For Ack.    ",  //AWAIT_SEIZED_ACKN
"Wait for Digits  ",  //SEIZED_ACKN
"Wait For PTS     ",  //AWAIT_DT_PTS
"PTS              ",  //DT_PTS
"Wtfr Routing     ",  //ADDRESSING_DGT
"Wait For EOS     ",  //AWAIT_DT_EOS
"EOS              ",  //DT_EOS
"Wait For Ans.    ",  //AWAIT_DT_ANSWERED
"Guard            ",  //DT_GUARD
"ICT Talk         ",  //EXT_DT_TALK
"Opr. Recall      ",  //DT_OPR_RCL_CLB
"Metering         ",  //DT_METERING
"Flash on OGT     ",  //OGDT_FLASH_DTCT
"OGT Block        ",  //DT_OG_BLOCK
"Local Block      ",  //DT_LOCAL_BLOCK
"Wait for Dgt.    ",  //AWAIT_DT_ADDRESSING , wtfr first digit
"???              ",  //AWAIT_DEST_ADDRESS
"Ring to Sub.     ",  //DT_EXT_RINGING
"ICT Talk         ",  //DT_EXT_TALK
"Flash on ICT     ",  //ICDT_FLASH_DTCT
"Wait For Idle    ",  //AWAIT_DT_CLF
"Wait for Ack.    ",  //AWAIT_SEIZED_ACKN_DGT_READY
"Wait for Dgt.    ",  //AWAIT_DEST_ADDRESSING, wtfr subscriber number
"Wait for Dgt.    ",  //AWAIT_CAS3_OPR_NUM
"FAS              ",  //DT_FAS
"OGT Talk, MLC    "   //EXT_DT_TALK_MALICIOUS
#endif

};

//EM_RECHECK start
/*
#ifdef LIM
int color[NO_EM_STATES+NO_CAS3_STATES]={
#else
int color[NO_EM_STATES]={
#endif
*/

//DOOD start
#if 0
int color[EMSTATE_NAME_ARR_SIZE]={

clr(BLACK,WHITE,DIM)<<8    ,    //      AWAIT_ACK
YELLOW<<8          ,    //      ACK
WHITE<<8           ,    //      OGT_BLOCK
WHITE<<8           ,    //      ICT_BLOCK
WHITE<<8           ,    //      AWAIT_SEIZE
WHITE<<8           ,    //      GAURD
WHITE<<8           ,    //      ANSWER
WHITE<<8           ,    //      AWAIT_DGT
WHITE<<8           ,    //      AWAIT_SEIZE_ACK
WHITE<<8           ,    //      IC_SGNL_BLOCK
WHITE<<8           ,    //      CONGESTION
AQUA <<8           ,    //      STD_CONNECT
WHITE<<8           ,    //      CLR_BACK
AQUA <<8           ,    //      TALK
RED  <<8           ,    //      METERING
WHITE<<8           ,    //      OPR_AWAIT_ACK_EM
WHITE<<8           ,    //      OPR_ACK_EM
WHITE<<8           ,    //      OPR_ANSWER_EM
WHITE<<8           ,    //      OPR_TALK_EM
WHITE<<8           ,    //      OPR_METERING_EM
WHITE<<8           ,    //      FLASH_DTCT_OGT_EM_PULSE
WHITE<<8           ,    //      FLASH_DTCT_ICT_EM_PULSE
WHITE<<8           ,    //      EM_FAS
WHITE<<8           ,    //      INIT_GAURD
BLUE <<8           ,    //      LOCAL_BLOCK
WHITE<<8           ,    //      AWAIT_1ST_DGT_EM
WHITE<<8           ,    //      ACK_INPUT_CLF
WHITE<<8           ,    //      TALK_INPUT_CLF
AQUA <<8                //      STD_CONNECT_RINGING
//EM_RECHECK start
#ifdef RMT_EM
,
RED<<8             ,    //      AWAIT_EM_LINK_DISCONNECT_ACK
RED<<8             ,    //      AWAIT_EM_LCL_BLK_LINK_DISCONNECT_ACK
RED<<8                  //      AWAIT_EM_RMT_BLK_LINK_DISCONNECT_ACK
#endif
//EM_RECHECK end
#ifdef LIM
,
clr(BLACK,WHITE,DIM)<<8    ,   //AWAIT_SEIZED_ACKN
YELLOW<<8   ,   //SEIZED_ACKN
WHITE<<8    ,   //AWAIT_DT_PTS
WHITE<<8    ,   //DT_PTS
WHITE<<8    ,   //ADDRESSING_DGT
WHITE<<8    ,   //AWAIT_DT_EOS
WHITE<<8    ,   //DT_EOS
WHITE<<8    ,   //AWAIT_DT_ANSWERED
BLUE<<8     ,   //DT_GUARD
AQUA<<8     ,   //EXT_DT_TALK
WHITE<<8    ,   //DT_OPR_RCL_CLB
RED<<8      ,   //DT_METERING
WHITE<<8    ,   //OGDT_FLASH_DTCT
WHITE<<8    ,   //DT_OG_BLOCK
WHITE<<8    ,   //DT_LOCAL_BLOCK
WHITE<<8    ,   //AWAIT_DT_ADDRESSING
WHITE<<8    ,   //AWAIT_DEST_ADDRESS
WHITE<<8    ,   //DT_EXT_RINGING
AQUA<<8     ,   //DT_EXT_TALK
WHITE<<8    ,   //ICDT_FLASH_DTCT
WHITE<<8    ,   //AWAIT_DT_CLF
WHITE<<8    ,   //AWAIT_SEIZED_ACKN_DGT_READY
WHITE<<8    ,   //AWAIT_DEST_ADDRESSING
WHITE<<8    ,   //AWAIT_CAS3_OPR_NUM
WHITE<<8    ,   //DT_FAS
WHITE<<8        //EXT_DT_TALK_MALICIOUS
#endif

};
#endif // 0
CHAT_TYPE color[EMSTATE_NAME_ARR_SIZE]={

MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),    //      AWAIT_ACK
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_YELLOW),   //      ACK
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),    //      OGT_BLOCK
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),    //      ICT_BLOCK
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),    //      AWAIT_SEIZE
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),    //      GAURD
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),    //      ANSWER
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),    //      AWAIT_DGT
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),    //      AWAIT_SEIZE_ACK
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),    //      IC_SGNL_BLOCK
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),    //      CONGESTION
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_CYAN),     //      STD_CONNECT
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),    //      CLR_BACK
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_CYAN),     //      TALK
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED),      //      METERING
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),    //      OPR_AWAIT_ACK_EM
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),    //      OPR_ACK_EM
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),    //      OPR_ANSWER_EM
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),    //      OPR_TALK_EM
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),    //      OPR_METERING_EM
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),    //      FLASH_DTCT_OGT_EM_PULSE
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),    //      FLASH_DTCT_ICT_EM_PULSE
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),    //      EM_FAS
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),    //      INIT_GAURD
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_BLUE),     //      LOCAL_BLOCK
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),    //      AWAIT_1ST_DGT_EM
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),    //      ACK_INPUT_CLF
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),    //      TALK_INPUT_CLF
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_CYAN )     //      STD_CONNECT_RINGING
//EM_RECHECK start
#ifdef RMT_EM
,
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED),    //      AWAIT_EM_LINK_DISCONNECT_ACK
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED),    //      AWAIT_EM_LCL_BLK_LINK_DISCONNECT_ACK
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED)//      AWAIT_EM_RMT_BLK_LINK_DISCONNECT_ACK
#endif
//EM_RECHECK end
#ifdef LIM
,
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //AWAIT_SEIZED_ACKN
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_YELLOW),   //SEIZED_ACKN
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //AWAIT_DT_PTS
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //DT_PTS
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //ADDRESSING_DGT
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //AWAIT_DT_EOS
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //DT_EOS
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //AWAIT_DT_ANSWERED
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_BLUE),   //DT_GUARD
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_CYAN),   //EXT_DT_TALK
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //DT_OPR_RCL_CLB
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED),   //DT_METERING
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //OGDT_FLASH_DTCT
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //DT_OG_BLOCK
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //DT_LOCAL_BLOCK
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //AWAIT_DT_ADDRESSING
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //AWAIT_DEST_ADDRESS
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //DT_EXT_RINGING
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_CYAN),   //DT_EXT_TALK
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //ICDT_FLASH_DTCT
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //AWAIT_DT_CLF
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //AWAIT_SEIZED_ACKN_DGT_READY
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //AWAIT_DEST_ADDRESSING
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //AWAIT_CAS3_OPR_NUM
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //DT_FAS
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE)//EXT_DT_TALK_MALICIOUS
#endif

};
//DOOD end
#endif

#ifdef CAS3_INSTALLED
#ifdef DISPLAY_CAS3_STATUS
BYTE dcsf;

char *ceptstate_name[NO_CAS3_STATES]={

"Wait For Ack.",  //AWAIT_SEIZED_ACKN
"Ack. Received",  //SEIZED_ACKN
"Wait For PTS.",  //AWAIT_DT_PTS
"PTS..........",  //DT_PTS
"Wtfr Routing ",  //ADDRESSING_DGT
"Wait For EOS ",  //AWAIT_DT_EOS
"EOS..........",  //DT_EOS
"Wait For Ans.",  //AWAIT_DT_ANSWERED
"Guard........",  //DT_GUARD
"OGT Talk.....",  //EXT_DT_TALK
"Opr. Recall..",  //DT_OPR_RCL_CLB
"Metering.....",  //DT_METERING
"Flash on OGT ",  //OGDT_FLASH_DTCT
"OGT Block....",  //DT_OG_BLOCK
"Local Block..",  //DT_LOCAL_BLOCK
"Wait for Dgt.",  //AWAIT_DT_ADDRESSING , wtfr first digit
"???..........",  //AWAIT_DEST_ADDRESS
"Ring to Sub..",  //DT_EXT_RINGING
"ICT Talk.....",  //DT_EXT_TALK
"Flash on ICT.",  //ICDT_FLASH_DTCT
"Wait For Idle",  //AWAIT_DT_CLF
"Wait for Ack.",  //AWAIT_SEIZED_ACKN_DGT_READY
"Wait for Dgt.",  //AWAIT_DEST_ADDRESSING, wtfr subscriber number
"Wait for Dgt.",  //AWAIT_CAS3_OPR_NUM
"FAS..........",  //DT_FAS
"OGT Talk, MLC"   //EXT_DT_TALK_MALICIOUS
};

//DOOD start
#if 0
int cas3_color[NO_CAS3_STATES]={

clr(BLACK,WHITE,DIM)<<8    ,   //AWAIT_SEIZED_ACKN
YELLOW<<8   ,   //SEIZED_ACKN
WHITE<<8    ,   //AWAIT_DT_PTS
WHITE<<8    ,   //DT_PTS
WHITE<<8    ,   //ADDRESSING_DGT
WHITE<<8    ,   //AWAIT_DT_EOS
WHITE<<8    ,   //DT_EOS
WHITE<<8    ,   //AWAIT_DT_ANSWERED
BLUE<<8     ,   //DT_GUARD
AQUA<<8     ,   //EXT_DT_TALK
WHITE<<8    ,   //DT_OPR_RCL_CLB
RED<<8      ,   //DT_METERING
WHITE<<8    ,   //OGDT_FLASH_DTCT
WHITE<<8    ,   //DT_OG_BLOCK
WHITE<<8    ,   //DT_LOCAL_BLOCK
WHITE<<8    ,   //AWAIT_DT_ADDRESSING
WHITE<<8    ,   //AWAIT_DEST_ADDRESS
WHITE<<8    ,   //DT_EXT_RINGING
AQUA<<8     ,   //DT_EXT_TALK
WHITE<<8    ,   //ICDT_FLASH_DTCT
WHITE<<8    ,   //AWAIT_DT_CLF
WHITE<<8    ,   //AWAIT_SEIZED_ACKN_DGT_READY
WHITE<<8    ,   //AWAIT_DEST_ADDRESSING
WHITE<<8    ,   //AWAIT_CAS3_OPR_NUM
WHITE<<8    ,   //DT_FAS
WHITE<<8        //EXT_DT_TALK_MALICIOUS
};
#endif // 0
CHAT_TYPE cas3_color[NO_CAS3_STATES]={

MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //AWAIT_SEIZED_ACKN
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_YELLOW),   //SEIZED_ACKN
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //AWAIT_DT_PTS
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //DT_PTS
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //ADDRESSING_DGT
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //AWAIT_DT_EOS
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //DT_EOS
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //AWAIT_DT_ANSWERED
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_BLUE),   //DT_GUARD
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_CYAN),   //EXT_DT_TALK
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //DT_OPR_RCL_CLB
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED),   //DT_METERING
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //OGDT_FLASH_DTCT
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //DT_OG_BLOCK
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //DT_LOCAL_BLOCK
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //AWAIT_DT_ADDRESSING
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //AWAIT_DEST_ADDRESS
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //DT_EXT_RINGING
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_CYAN),   //DT_EXT_TALK
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //ICDT_FLASH_DTCT
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //AWAIT_DT_CLF
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //AWAIT_SEIZED_ACKN_DGT_READY
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //AWAIT_DEST_ADDRESSING
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //AWAIT_CAS3_OPR_NUM
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),   //DT_FAS
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE)     //EXT_DT_TALK_MALICIOUS
};
//DOOD end

#ifdef ISUP

char *isupstate_name[NO_ISUP_STATES]={

".............",  //ISUPI_WAIT_FOR_FURTHER_DIGITS_STATE
".............",  //ISUPI_EXT_RINGING_STATE
".............",  //ISUPI_ANSWERED_STATE
".............",  //ISUPI_WAIT_FOR_RLC_STATE
".............",  //ISUPI_WAIT_FOR_REANSWER_STATE
"Wait For Dgt.",  //ISUPO_WAIT_FOR_DIGITS_STATE
"Wait For ACM.",  //ISUPO_WAIT_FOR_ACM_STATE
"Wait For ANM.",  //ISUPO_WAIT_FOR_ANM_STATE
".............",  //ISUPO_WAIT_FOR_RLC_STATE
"->ISUP Talk..",  //ISUPO_ANSWERED_STATE
".............",  //ISUPI_WAIT_FOR_CONTINUITY_CHECK_STATE
"Wait For Dgts",  //ISUPO_WAIT_FOR_MORE_DIGITS_STATE
".............",  //HEAR_ANNOUNCEMENT
".............",  //ISUPI_FLASH_DETECTED_STATE
"............."   //ISUPO_FLASH_DETECTED_STATE
};

//DOOD start
#if 0
int isup_color[NO_ISUP_STATES]={

clr(BLACK,WHITE,DIM)<<8,       //ISUPI_WAIT_FOR_FURTHER_DIGITS_STATE
RED<<8      ,                  //ISUPI_EXT_RINGING_STATE
RED<<8      ,                  //ISUPI_ANSWERED_STATE
RED<<8      ,                  //ISUPI_WAIT_FOR_RLC_STATE
RED<<8      ,                  //ISUPI_WAIT_FOR_REANSWER_STATE
YELLOW<<8   ,                  //ISUPO_WAIT_FOR_DIGITS_STATE
YELLOW<<8   ,                  //ISUPO_WAIT_FOR_ACM_STATE
WHITE<<8    ,                  //ISUPO_WAIT_FOR_ANM_STATE
RED<<8      ,                  //ISUPO_WAIT_FOR_RLC_STATE
AQUA<<8     ,                  //ISUPO_ANSWERED_STATE
RED<<8      ,                  //ISUPI_WAIT_FOR_CONTINUITY_CHECK_STATE
YELLOW<<8   ,                  //ISUPO_WAIT_FOR_MORE_DIGITS_STATE
RED<<8      ,                  //HEAR_ANNOUNCEMENT
RED<<8      ,                  //ISUPI_FLASH_DETECTED_STATE
RED<<8                         //ISUPO_FLASH_DETECTED_STATE
};
#endif // 0
CHAT_TYPE isup_color[NO_ISUP_STATES]={

MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),       //ISUPI_WAIT_FOR_FURTHER_DIGITS_STATE
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED),                  //ISUPI_EXT_RINGING_STATE
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED),                  //ISUPI_ANSWERED_STATE
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED),                  //ISUPI_WAIT_FOR_RLC_STATE
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED),                  //ISUPI_WAIT_FOR_REANSWER_STATE
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_YELLOW),                  //ISUPO_WAIT_FOR_DIGITS_STATE
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_YELLOW),                  //ISUPO_WAIT_FOR_ACM_STATE
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_WHITE),                  //ISUPO_WAIT_FOR_ANM_STATE
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED),                  //ISUPO_WAIT_FOR_RLC_STATE
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_CYAN),                  //ISUPO_ANSWERED_STATE
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED),                  //ISUPI_WAIT_FOR_CONTINUITY_CHECK_STATE
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_YELLOW),                  //ISUPO_WAIT_FOR_MORE_DIGITS_STATE
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED),                  //HEAR_ANNOUNCEMENT
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED),                  //ISUPI_FLASH_DETECTED_STATE
MAKE_ATTR(KT_NO_BLINKING,KT_BLACK,KT_LOW_INTENSITY,KT_RED)                         //ISUPO_FLASH_DETECTED_STATE
};
//DOOD end
#endif
#endif
#endif


#ifdef SYSTEM_MANUAL_TEST
BYTE mtf;
#endif
#ifdef NEW_DISPLAY_STATUS
BYTE current_page;
#endif
#ifdef DISPLAY_CAS3_STATUS
BYTE current_cas3_page;
#endif
#ifdef DISPLAY_BILLING
BYTE bill_display_flag;
BYTE bill_line;
BYTE scroll_flag;
#endif

#if ((defined HDLC_RS232_ON_OPERATOR_SIDE) || (defined TCPIP_ON_SYS_OAM_OPERATOR_SIDE) || (defined MY_TCPIP_FOR_OAM) || (defined MY_UDP_FOR_OAM))
BYTE hot_link;
#endif
#ifdef HOT_STANDBY
BYTE htby_link_flag;
WINDOW *htby_win;
#endif
#ifdef PARAMETER_DOWN_UP_LOAD

#ifdef OPR_UPLOAD_WITH_TIMER
unsigned int upload_count;
#endif

#if ((NO_OPRTR > 1) && (defined HOT_STANDBY))
unsigned short upload_reject_flag;
#endif

#endif

#ifdef DISPLAY_ALARM
BYTE alarm_display_flag;
BYTE alarm_line;
BYTE alarm_scroll_flag;
#endif
unsigned int update_time_index;

//#if ((defined CALL_PROCESSOR) && (defined EXT_INSTALLED))
unsigned int no_of_subscriber_groups;
SUBSCRIBER_GROUP subscriber_group[MAX_NO_OF_SUBSCRIBER_GROUPS];
//#endif

char exchange_name[100];
char exchange_office_code[100];
char exchange_area_code[100];
SITE_PARAMETERS site_data;
