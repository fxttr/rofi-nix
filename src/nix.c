/**
 * rofi-nix
 *
 * MIT License
 * Copyright (c) 2024 Florian Marrero Liestmann <f.m.liestmann@fx-ttr.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <gmodule.h>

#include <rofi/mode.h>
#include <rofi/helper.h>
#include <rofi/mode-private.h>

#include <stdint.h>

G_MODULE_EXPORT Mode mode;

/**
 * The internal data structure holding the private data of the TEST Mode.
 */
typedef struct
{
    char **array;
    unsigned int array_length;
} rofinixModePrivateData;


static void get_rofinix (  Mode *sw )
{
    /** 
     * Get the entries to display.
     * this gets called on plugin initialization.
     */
}


static int rofinix_mode_init ( Mode *sw )
{
    /**
     * Called on startup when enabled (in modi list)
     */
    if ( mode_get_private_data ( sw ) == NULL ) {
        rofi-nixModePrivateData *pd = g_malloc0 ( sizeof ( *pd ) );
        mode_set_private_data ( sw, (void *) pd );
        // Load content.
        get_rofi-nix ( sw );
    }
    return TRUE;
}
static unsigned int rofinix_mode_get_num_entries ( const Mode *sw )
{
    const rofi-nixModePrivateData *pd = (const rofi-nixModePrivateData *) mode_get_private_data ( sw );
    return pd->array_length;
}

static ModeMode rofinix_mode_result ( Mode *sw, int mretv, char **input, unsigned int selected_line )
{
    ModeMode           retv  = MODE_EXIT;
    rofi-nixModePrivateData *pd = (rofi-nixModePrivateData *) mode_get_private_data ( sw );
    if ( mretv & MENU_NEXT ) {
        retv = NEXT_DIALOG;
    } else if ( mretv & MENU_PREVIOUS ) {
        retv = PREVIOUS_DIALOG;
    } else if ( mretv & MENU_QUICK_SWITCH ) {
        retv = ( mretv & MENU_LOWER_MASK );
    } else if ( ( mretv & MENU_OK ) ) {
        retv = RELOAD_DIALOG;
    } else if ( ( mretv & MENU_ENTRY_DELETE ) == MENU_ENTRY_DELETE ) {
        retv = RELOAD_DIALOG;
    }
    return retv;
}

static void rofinix_mode_destroy ( Mode *sw )
{
    rofi-nixModePrivateData *pd = (rofi-nixModePrivateData *) mode_get_private_data ( sw );
    if ( pd != NULL ) {
        g_free ( pd );
        mode_set_private_data ( sw, NULL );
    }
}

static char *_get_display_value ( const Mode *sw, unsigned int selected_line, G_GNUC_UNUSED int *state, G_GNUC_UNUSED GList **attr_list, int get_entry )
{
    rofi-nixModePrivateData *pd = (rofi-nixModePrivateData *) mode_get_private_data ( sw );

    // Only return the string if requested, otherwise only set state.
    return get_entry ? g_strdup("n/a"): NULL; 
}

/**
 * @param sw The mode object.
 * @param tokens The tokens to match against.
 * @param index  The index in this plugin to match against.
 *
 * Match the entry.
 *
 * @param returns try when a match.
 */
static int rofi-nix_token_match ( const Mode *sw, rofi_int_matcher **tokens, unsigned int index )
{
    rofi-nixModePrivateData *pd = (rofi-nixModePrivateData *) mode_get_private_data ( sw );

    // Call default matching function.
    return helper_token_match ( tokens, pd->array[index]);
}


Mode mode =
{
    .abi_version        = ABI_VERSION,
    .name               = "rofi-nix",
    .cfg_name_key       = "display-rofi-nix",
    ._init              = rofinix_mode_init,
    ._get_num_entries   = rofinix_mode_get_num_entries,
    ._result            = rofinix_mode_result,
    ._destroy           = rofinix_mode_destroy,
    ._token_match       = rofinix_token_match,
    ._get_display_value = _get_display_value,
    ._get_message       = NULL,
    ._get_completion    = NULL,
    ._preprocess_input  = NULL,
    .private_data       = NULL,
    .free               = NULL,
};
