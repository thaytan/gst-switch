Imported #410806 from gst-plugins-bad

Run:
sed -i -e's/GstInter/DvsInter/g' \
   -e's/GST_TYPE_INTER/DVS_TYPE_INTER/g' \
   -e's/gst_inter_/dvs_inter_/g' *inter*.[ch]
to create GObjects with non-conflicting names

Run:
sed -i -e's/ \"inter/ \"dvsinter' gstinter.c

to register elements as 'dvsintervideosrc' etc instead of plain 'intervideosrc'
