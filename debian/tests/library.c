#include <xreader-document.h>
#include <xreader-view.h>

int main(int argc, char **argv)
{
    GError *error = NULL;
    gtk_init(&argc, &argv);
    g_assert_true(ev_init());
    gchar *uri = g_filename_to_uri(argv[1], NULL, &error);
    g_assert_no_error(error);
    EvDocument *document = ev_document_factory_get_document(uri, &error);
    g_assert_no_error(error);
    g_assert_nonnull(document);
    g_assert_cmpint(ev_document_get_n_pages(document), ==, 1);
    GtkWidget *view = ev_view_new();
    g_assert_true(EV_IS_VIEW(view));
    g_object_ref_sink(view);
    g_object_unref(view);
    g_object_unref(document);
    g_free(uri);
    ev_shutdown();
    return 0;
}
