CHECK_INCLUDE_FILE (gst/gst.h HAVE_GST_GST_H)

IF (NOT HAVE_GST_GST_H)
    DISABLE_PLUGIN ()
ENDIF (NOT HAVE_GST_GST_H)