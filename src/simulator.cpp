#include "simulator.hpp"

#include <cstddef>

namespace sim {
  /**
   * Constructor de la calse simulador
   * @param args_view: Span con el que se construye args_parser para la comprobacion de argumentos
   */
  Simulator::Simulator(std::span<char const *> const args_view)
    : args_parser_(args_view), nts_(0) { }

  /**
   * Representa la primera etapa de la simulacion, utilza la clase Proargs para comprobar que los
   * argumentos son correctos.
   * @return Devuelve SUCCESS (0) si los argumentos pasados por el usuario eran correctos o bien el
   * error correspondiente
   */
  sim::error_code Simulator::ParseArgs() {
    error_code err = success;

    err = args_parser_.CheckCount();
    if (err != success) {  //[[likely]] o [[unlikely]]?
      return (err);
    }

    err = args_parser_.CheckNts(nts_);
    if (err != 0) { return (err); }

    err = args_parser_.CheckOpenFiles(init_file_, final_file_);
    if (err != success) { return (err); }
    return (err);
  }

  /**
   * Representa la segunda fase de la simulacion, donde se inicializa la simulacion, se lee el
   * fichero de entrada y se inicializa el grid con dicha informacion
   * @return
   */
  sim::error_code Simulator::InitSim() {
    sim::error_code err = success;
    std::vector<Particle> particles;
    int num_particles = 0;
    math::scalar ppm        = 0.0;

    err = init_file_.ReadHeader(ppm, num_particles);
    if (err != success) { return (err); }

    particles = init_file_.ReadParticles();
    grid_.emplace(num_particles, ppm, particles);
    return (err);
  }

  /**
   * Representa la tercera fase de la simulacion, donde se procesan los calculos necesarios tantas
   * veces como el usuario indico por argumento
   * @return
   */
  sim::error_code Simulator::ProcessSim() {
    for (int i = 0; i < nts_; i++) {
      if (i > 0) {
        grid_->Repositioning();
      }
      grid_->CalculateAccelerations();
      grid_->ProcessCollisions();
      grid_->MoveParticles();
      grid_->ProcessLimits();
    }
    return (success);
  }

  /**
   * Representa la ultima fase de la simulacion, se almacenan los resultados en el fichero
   * especificado por parametro
   * @return
   */
  sim::error_code Simulator::StoreResults() {
    int const num_particles = grid_->GetNumParticles();
    std::vector<Particle *> results(num_particles);

    final_file_.WriteHeader(num_particles, grid_->GetParticlesPerMeter());

    for (auto & block : grid_->GetBlocks()) {
      for (auto & particle : block.GetParticles()) { results[particle.id] = &particle; }
    }

    final_file_.WriteParticles(results);
    return (success);
  }

}  // namespace sim
