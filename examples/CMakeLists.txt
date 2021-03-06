# Copyright (c) 2007-2009 Hartmut Kaiser
# Copyright (c) 2011      Bryce Lelbach
# Copyright (c) 2011      Dylan Stark
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

set(subdirs
    1d_hydro
    accumulator
    allgather
    async_io
    balancing
    cancelable_action
    future_reduce
    heartbeat
    interpolate1d
    jacobi
    jacobi_smp
    nqueen
    mini_ghost
    performance_counters
    quickstart
    qt
    random_mem_access
    sheneos
    spell_check
    startup_shutdown
    thread_aware_timer
    throttle
   )

if(HPX_HAVE_CXX11_LAMBDAS)
  set(subdirs ${subdirs}
      tuplespace
      diskperf
      )
endif()

foreach(subdir ${subdirs})
  add_hpx_pseudo_target(examples.${subdir})
  add_subdirectory(${subdir})
  add_hpx_pseudo_dependencies(examples examples.${subdir})
endforeach()

