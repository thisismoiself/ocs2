#pragma once

#include <ocs2_switched_model_interface/constraint/EndEffectorConstraint.h>

namespace switched_model {

using EndEffectorVelocityConstraintSettings = EndEffectorConstraintSettings;

class EndEffectorVelocityConstraint : public EndEffectorConstraint<EndEffectorVelocityConstraint> {
 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  using BASE = EndEffectorConstraint<EndEffectorVelocityConstraint>;
  using typename BASE::ad_com_model_t;
  using typename BASE::ad_dynamic_vector_t;
  using typename BASE::ad_interface_t;
  using typename BASE::ad_kinematic_model_t;
  using typename BASE::ad_scalar_t;
  using typename BASE::constraint_timeStateInput_matrix_t;
  using typename BASE::dynamic_vector_t;
  using typename BASE::input_matrix_t;
  using typename BASE::input_state_matrix_t;
  using typename BASE::input_vector_t;
  using typename BASE::LinearApproximation_t;
  using typename BASE::QuadraticApproximation_t;
  using typename BASE::scalar_array_t;
  using typename BASE::scalar_t;
  using typename BASE::state_matrix_t;
  using typename BASE::state_vector_t;
  using typename BASE::timeStateInput_matrix_t;

  explicit EndEffectorVelocityConstraint(int legNumber, EndEffectorVelocityConstraintSettings settings, ad_com_model_t& adComModel,
                                         ad_kinematic_model_t& adKinematicsModel, bool generateModels = true,
                                         std::string constraintPrefix = "EEVelocityConstraint_")
      : BASE(kConstraintOrder, std::move(constraintPrefix), legNumber, std::move(settings), adComModel, adKinematicsModel, generateModels) {
  }

  /* Copy Constructors */
  EndEffectorVelocityConstraint(const EndEffectorVelocityConstraint& rhs) : BASE(rhs){};

  EndEffectorVelocityConstraint* clone() const override { return new EndEffectorVelocityConstraint(*this); };

 private:
  friend BASE;
  void adfunc(ad_com_model_t& adComModel, ad_kinematic_model_t& adKinematicsModel, const ad_dynamic_vector_t& tapedInput,
              ad_dynamic_vector_t& o_footVelocity) {
    // Extract elements from taped input
    ad_scalar_t t = tapedInput(0);
    comkino_state_ad_t x = tapedInput.segment(1, STATE_DIM);
    comkino_input_ad_t u = tapedInput.segment(1 + STATE_DIM, INPUT_DIM);

    // Extract elements from state
    const base_coordinate_ad_t comPose = getComPose(x);
    const base_coordinate_ad_t com_comTwist = getComLocalVelocities(x);
    const joint_coordinate_ad_t qJoints = getJointPositions(x);
    const joint_coordinate_ad_t dqJoints = getJointVelocities(u);

    // Get base state from com state
    const base_coordinate_ad_t basePose = adComModel.calculateBasePose(comPose);
    const base_coordinate_ad_t com_baseTwist = adComModel.calculateBaseLocalVelocities(com_comTwist);

    o_footVelocity = adKinematicsModel.footVelocityInOriginFrame(this->legNumber_, basePose, com_baseTwist, qJoints, dqJoints);
  }

  static constexpr ocs2::ConstraintOrder kConstraintOrder = ocs2::ConstraintOrder::Linear;
};

}  // namespace switched_model
