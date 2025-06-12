/* This file is an image processing operation for GEGL
 *
 * GEGL is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * GEGL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GEGL; if not, see <https://www.gnu.org/licenses/>.
 *
 * Credit to Øvind Kolas (pippin) for major GEGL contributions
 * 2025, beaver, Frozen Gel

Syntax to recreate plugin (requires Glass Over Text and Edge Smooth)

noise-spread amount-x=10 amount-y=10
id=n1 

gaussian-blur std-dev-x=1 std-dev-y=1
color-overlay value=#52ebff

id=2
median-blur radius=0 abyss-policy=none
gimp:layer-mode layer-mode=overlay opacity=0.6 blend-space=rgb-perceptual aux=[ ref=2 wind direction=bottom style=blast threshold=4 seed=23 strength=10 oilify mask-radius=3   ]
id=3
id=edge
gimp:layer-mode layer-mode=overlay composite-mode=auto aux=[  ref=edge edge amount=10 gray ]



median-blur radius=0 abyss-policy=none
id=glass

over aux=[ ref=glass lb:glassovertext depth=30 hyperopacity=0.5 ]

id=4 gimp:layer-mode layer-mode=overlay opacity=0.2 blend-space=rgb-linear aux=[ ref=4 
wind direction=bottom style=blast threshold=1 seed=13 strength=41 oilify mask-radius=5
bevel blendmode=hardlight radius=2 type=bump opacity value=0.5 ]
id=5
gimp:layer-mode layer-mode=overlay aux=[ ref=5 edge gray ]
saturation scale=0.45

id=6

over aux=[ ref=6 

gimp:layer-mode layer-mode=hsv-value blend-space=rgb-linear opacity=0.3 aux=[ ref=6 color-overlay value=#00d8ff ]
opacity value=0.4
]

lb:edgesmooth gaus=0 value=1 abyss-policy=none
gimp:layer-mode layer-mode=split opacity=0.20 aux=[ ref=n1 gaussian-blur   ]


end of syntax
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

/*Properties go here*/

property_int (jag, _("Jaggedness"),  10)
   description  (_("Internal noise that determines how rough the ice bevel's structure will be"))
   value_range  (1, 20)


property_double (azimuth, _("Azimuth rotation of the ice's light"), 30.0)
    description (_("Light angle (degrees)"))
    value_range (0, 360)
    ui_meta ("unit", "degree")
    ui_meta ("direction", "ccw")
  ui_steps      (0.5, 0.50)

property_int (icicle, _("Icicle drip length"),  41)
   description  (_("The length of the icicle drip"))
   value_range  (5, 100)


property_double (detail, _("Icicle detail visibility"),  0.5)
   description  (_("Icicle details become more prominent on higher values"))
   value_range  (0.3, 1.0)
   ui_range  (0.3, 0.6)

property_seed    (seed, _("Icicle seed"), rand)
    description  (_("The random seed for the icicle noise function"))

property_double (transparency, _("Transparency"),  0.20)
   description  (_("Transparency of text style"))
   value_range  (0.0, 1.0)
   ui_range  (0.0, 0.2)

property_double (hue, _("Color Rotation"),  0.0)
   description  (_("Hue rotation to change the text styles color"))
   value_range  (-180.0, 180.0)

property_double (lightness, _("Lightness"), 0.0)
   description  (_("Lightness boost for the totality of the text style"))
   value_range  (-15.0, 15.0)

property_double (saturation, _("Saturation"), 1.0)
   description  (_("Saturation boost for the totality of the text style"))
   value_range  (0.0, 3.0)

#else

#define GEGL_OP_META
#define GEGL_OP_NAME     frozengel
#define GEGL_OP_C_SOURCE frozengel.c

#include "gegl-op.h"

/*starred nodes go inside typedef struct */

typedef struct
{
 GeglNode *input;
 GeglNode *noise;
 GeglNode *nop1;
 GeglNode *spliterase;
 GeglNode *nop2;
 GeglNode *nop3;
 GeglNode *color;
 GeglNode *graph1;
 GeglNode *graph2;
 GeglNode *normal; 
 GeglNode *idrefglass;
 GeglNode *glasstext; 
 GeglNode *overlay; 
 GeglNode *wind; 
 GeglNode *gaus1; 
 GeglNode *gaus2; 
 GeglNode *oilify; 
 GeglNode *bevel;
 GeglNode *huelight;
 GeglNode *opacity;
 GeglNode *saturation;  
 
 GeglNode *output;
}State;

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);
  GeglColor *lightblue = gegl_color_new ("#52ebff");

  State *state = o->user_data = g_malloc0 (sizeof (State));

/*new child node list is here, this is where starred nodes get defined

 state->newchildname = gegl_node_new_child (gegl, "operation", "lb:name", NULL);*/
  state->input    = gegl_node_get_input_proxy (gegl, "input");
  state->output   = gegl_node_get_output_proxy (gegl, "output");

#define graphone \
" id=2 median-blur radius=0 abyss-policy=none gimp:layer-mode layer-mode=overlay opacity=0.6 blend-space=rgb-perceptual aux=[ ref=2 wind direction=bottom style=blast threshold=4 seed=23 strength=10 oilify mask-radius=3   ] id=3 id=edge gimp:layer-mode layer-mode=overlay composite-mode=auto aux=[  ref=edge edge amount=10 gray ] median-blur radius=0 abyss-policy=none "\


#define graphtwo \
" id=5 gimp:layer-mode layer-mode=overlay aux=[ ref=5 edge gray ] saturation scale=0.45  id=6 over aux=[ ref=6  gimp:layer-mode layer-mode=hsv-value blend-space=rgb-linear opacity=0.3 aux=[ ref=6 color-overlay value=#00d8ff ] opacity value=0.4 ] lb:edgesmooth gaus=0 value=1 abyss-policy=none  "\


 state->noise = gegl_node_new_child (gegl, "operation", "gegl:noise-spread", "amount-x", 10, "amount-y", 10, NULL);
 state->nop1 = gegl_node_new_child (gegl, "operation", "gegl:nop", NULL);
 state->nop2 = gegl_node_new_child (gegl, "operation", "gegl:nop", NULL);
 state->nop3 = gegl_node_new_child (gegl, "operation", "gegl:nop", NULL);
 state->spliterase = gegl_node_new_child (gegl, "operation", "gimp:layer-mode", "layer-mode", 60, "opacity", 0.2,  NULL);
 state->normal = gegl_node_new_child (gegl, "operation", "gegl:over", NULL);
 state->glasstext = gegl_node_new_child (gegl, "operation", "lb:glassovertext", "depth", 30, "hyperopacity", 0.5, NULL);
 state->gaus1 = gegl_node_new_child (gegl, "operation", "gegl:gaussian-blur", "std-dev-x", 1.0,  "std-dev-y", 1.0, "abyss-policy", 0, "clip-extent", FALSE, NULL);
 state->gaus2 = gegl_node_new_child (gegl, "operation", "gegl:gaussian-blur", "std-dev-x", 1.0,  "std-dev-y", 1.0, "abyss-policy", 0, "clip-extent", FALSE, NULL);
 state->saturation = gegl_node_new_child (gegl, "operation", "gegl:saturation", NULL);
 state->huelight = gegl_node_new_child (gegl, "operation", "gegl:hue-chroma", NULL);
 state->opacity = gegl_node_new_child (gegl, "operation", "gegl:opacity", "value", 0.6, NULL);
 state->overlay = gegl_node_new_child (gegl, "operation", "gimp:layer-mode", "layer-mode", 23, "opacity", 0.2, "blend-space", 1, NULL);
 state->oilify = gegl_node_new_child (gegl, "operation", "gegl:oilify", "mask-radius", 5, NULL);
 state->wind = gegl_node_new_child (gegl, "operation", "gegl:wind", "style", 1, "threshold", 1, "strength", 41, "direction", 3, NULL);
 state->bevel = gegl_node_new_child (gegl, "operation", "gegl:bevel", "blendmode", 1, "radius", 2.0, "type", 1,  NULL);
 state->graph1 = gegl_node_new_child (gegl, "operation", "gegl:gegl", "string", graphone, NULL);
 state->graph2 = gegl_node_new_child (gegl, "operation", "gegl:gegl", "string", graphtwo, NULL);
 state->color = gegl_node_new_child (gegl, "operation", "gegl:color-overlay", "value", lightblue, NULL);
/*meta redirect property to new child orders go here

 gegl_operation_meta_redirect (operation, "propertyname", state->newchildname,  "originalpropertyname");
*/

 gegl_operation_meta_redirect (operation, "icicle", state->wind,  "strength");
 gegl_operation_meta_redirect (operation, "lightness", state->huelight,  "lightness");
 gegl_operation_meta_redirect (operation, "hue", state->huelight,  "hue");
 gegl_operation_meta_redirect (operation, "saturation", state->saturation,  "scale");
 gegl_operation_meta_redirect (operation, "azimuth", state->glasstext,  "azimuth");
 gegl_operation_meta_redirect (operation, "seed", state->wind,  "seed");
 gegl_operation_meta_redirect (operation, "jag", state->noise,  "amount-x");
 gegl_operation_meta_redirect (operation, "jag", state->noise,  "amount-y");
 gegl_operation_meta_redirect (operation, "detail", state->opacity,  "value");
 gegl_operation_meta_redirect (operation, "transparency", state->spliterase,  "opacity");
}

static void
update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  if (!state) return;



 gegl_node_link_many (state->input, state->noise, state->nop1, state->gaus1, state->color, state->graph1, state->nop2, state->normal, state->nop3, state->overlay, state->graph2, state->huelight, state->saturation, state->spliterase,  state->output,  NULL);
 gegl_node_connect (state->normal, "aux", state->glasstext, "output");
 gegl_node_connect (state->overlay, "aux", state->opacity, "output");
 gegl_node_link_many (state->nop3, state->wind, state->oilify, state->bevel, state->opacity,   NULL);
 gegl_node_link_many (state->nop2, state->glasstext,  NULL);
 gegl_node_connect (state->spliterase, "aux", state->gaus2, "output");
 gegl_node_link_many (state->nop1,  state->gaus2,  NULL);


}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;
GeglOperationMetaClass *operation_meta_class = GEGL_OPERATION_META_CLASS (klass);
  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;
  operation_meta_class->update = update_graph;

  gegl_operation_class_set_keys (operation_class,
    "name",        "lb:frozen-gel",
    "title",       _("Frozen Gel"),
    "reference-hash", "gegologysessionicecoldedition",
    "description", _("Icey cold frozen text"),
/*<Image>/Colors <Image>/Filters are top level menus in GIMP*/
    "gimp:menu-path", "<Image>/Filters/Text Styling",
    "gimp:menu-label", _("Frozen Gel..."),
    NULL);
}

#endif
