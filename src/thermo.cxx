/*
 * MicroHH
 * Copyright (c) 2011-2017 Chiel van Heerwaarden
 * Copyright (c) 2011-2017 Thijs Heus
 * Copyright (c) 2014-2017 Bart van Stratum
 *
 * This file is part of MicroHH
 *
 * MicroHH is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * MicroHH is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with MicroHH.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstdio>
#include "master.h"
#include "grid.h"
#include "fields.h"
#include "defines.h"
#include "model.h"

// thermo schemes
#include "thermo.h"
#include "thermo_buoy.h"
#include "thermo_dry.h"
#include "thermo_moist.h"
#include "thermo_vapor.h"
#include "thermo_disabled.h"

Thermo::Thermo(Model* modelin, Input* inputin)
{
    model  = modelin;
    grid   = model->grid;
    fields = model->fields;
    master = model->master;
}

Thermo::~Thermo()
{
}

std::string Thermo::get_switch()
{
    return swthermo;
}

Thermo* Thermo::factory(Master* masterin, Input* inputin, Model* modelin)
{
    std::string swthermo;
    if (inputin->get_item(&swthermo, "thermo", "swthermo", "", "0"))
        return 0;

    if (swthermo == "moist")
        return new Thermo_moist(modelin, inputin);
    else if (swthermo == "vapor")
        return new Thermo_vapor(modelin, inputin);
    else if (swthermo == "buoy")
        return new Thermo_buoy(modelin, inputin);
    else if (swthermo == "dry")
        return new Thermo_dry(modelin, inputin);
    else if (swthermo == "0")
        return new Thermo_disabled(modelin, inputin);
    else
    {
        masterin->print_error("\"%s\" is an illegal value for swthermo\n", swthermo.c_str());
        throw 1;
    }
}
