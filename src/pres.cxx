#include <cstdio>
#include <cmath>
#include <algorithm>
#include <fftw3.h>
#include "grid.h"
#include "fields.h"
#include "pres.h"
#include "pres_g2.h"
#include "defines.h"

cpres::cpres(cgrid *gridin, cfields *fieldsin, cmpi *mpiin)
{
  std::printf("Creating instance of object pres\n");
  grid   = gridin;
  fields = fieldsin;
  mpi    = mpiin;

  pres_g2 = new cpres_g2(grid, fields, mpi);
}

cpres::~cpres()
{
  delete pres_g2;

  std::printf("Destroying instance of object pres\n");
}

int cpres::readinifile(cinput *inputin)
{
  // input parameters
  int n = 0;

  // obligatory parameters
  n += inputin->getItem(&ipres, "physics", "ipres");

  // if one argument fails, then crash
  if(n > 0)
    return 1;

  return 0;
}

int cpres::init()
{
  pres_g2->init();

  return 0;
}

int cpres::exec(double dt)
{
  if(ipres == 0)
    return 0;

  if(ipres == 2)
  {
    // create the input for the pressure solver
    pres_g2->pres_in((*fields->p ).data,
                     (*fields->u ).data, (*fields->v ).data, (*fields->w ).data,
                     (*fields->ut).data, (*fields->vt).data, (*fields->wt).data, 
                     grid->dzi, dt);

    // solve the system
    pres_g2->pres_solve((*fields->p).data, (*fields->tmp1).data, (*fields->tmp2).data, grid->dz,
                        pres_g2->fftini, pres_g2->fftouti, pres_g2->fftinj, pres_g2->fftoutj);

    // get the pressure tendencies from the pressure field
    pres_g2->pres_out((*fields->ut).data, (*fields->vt).data, (*fields->wt).data, 
                      (*fields->p ).data, grid->dzhi);
  }

  return 0;
}

double cpres::check()
{
  double divmax;
  divmax = pres_g2->calcdivergence((*fields->u).data, (*fields->v).data, (*fields->w).data, grid->dzi);

  return divmax;
}

int cpres::load()
{
  if(ipres == 2)
    return pres_g2->load();

  return 0;
}

int cpres::save()
{
  if(ipres == 2)
    return pres_g2->save();

  return 0;
}

