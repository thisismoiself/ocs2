/******************************************************************************
Copyright (c) 2020, Farbod Farshidian. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the copyright holder nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
******************************************************************************/

#pragma once

#include <ocs2_core/integration/IntegratorBase.h>

namespace ocs2 {

class RungeKuttaDormandPrince5 : public IntegratorBase {
 public:
  explicit RungeKuttaDormandPrince5(std::shared_ptr<SystemEventHandler> eventHandlerPtr = nullptr)
      : IntegratorBase(std::move(eventHandlerPtr)){};

  ~RungeKuttaDormandPrince5() override = default;

 private:
  /**
   * Equidistant integration based on initial and final time as well as step length.
   *
   * @param [in] system: System function
   * @param [in] observer: Observer callback
   * @param [in] initialState: Initial state.
   * @param [in] startTime: Initial time.
   * @param [in] finalTime: Final time.
   * @param [in] dt: Time step.
   */
  void runIntegrateConst(system_func_t system, observer_func_t observer, const vector_t& initialState, scalar_t startTime,
                         scalar_t finalTime, scalar_t dt) override {
    throw std::runtime_error("not implemented");
  }

  /**
   * Adaptive time integration based on start time and final time.
   *
   * @param [in] system: System function
   * @param [in] observer: Observer callback
   * @param [in] initialState: Initial state.
   * @param [in] startTime: Initial time.
   * @param [in] finalTime: Final time.
   * @param [in] dtInitial: Initial time step.
   * @param [in] absTol: The absolute tolerance error for ode solver.
   * @param [in] relTol: The relative tolerance error for ode solver.
   */
  void runIntegrateAdaptive(system_func_t system, observer_func_t observer, const vector_t& initialState, scalar_t startTime,
                            scalar_t finalTime, scalar_t dtInitial, scalar_t absTol, scalar_t relTol) override;
  /**
   * Output integration based on a given time trajectory.
   *
   * @param [in] system: System function
   * @param [in] observer: Observer callback
   * @param [in] initialState: Initial state.
   * @param [in] beginTimeItr: The iterator to the beginning of the time stamp trajectory.
   * @param [in] endTimeItr: The iterator to the end of the time stamp trajectory.
   * @param [in] dtInitial: Initial time step.
   * @param [in] absTol: The absolute tolerance error for ode solver.
   * @param [in] relTol: The relative tolerance error for ode solver.
   */
  void runIntegrateTimes(system_func_t system, observer_func_t observer, const vector_t& initialState,
                         typename scalar_array_t::const_iterator beginTimeItr, typename scalar_array_t::const_iterator endTimeItr,
                         scalar_t dtInitial, scalar_t absTol, scalar_t relTol) override;

 private:
  bool tryStep(system_func_t& system, vector_t& x, vector_t& dxdt, scalar_t& t, scalar_t& dt, scalar_t absTol, scalar_t relTol);

  void doStep(system_func_t& system, const vector_t& x0, const vector_t& dxdt, scalar_t t, scalar_t dt, vector_t& x_out, vector_t& dxdt_out,
              vector_t& error);

  scalar_t decrease_step(scalar_t dt, scalar_t error) const;

  scalar_t increase_step(scalar_t dt, scalar_t error) const;

  scalar_t error(const vector_t& x_old, const vector_t& dxdt_old, vector_t& x_err, scalar_t dt, scalar_t eps_abs, scalar_t eps_rel) const;

 private:
  const size_t MAX_STEP_RETRIES = 100;

  vector_t x_;
  vector_t k1_, k2_, k3_, k4_, k5_, k6_;
};

}  // namespace ocs2
