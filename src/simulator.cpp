#include "simulator.hpp"

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
  error_code Simulator::parseArgs() {
    error_code err = success;

    err = args_parser_.CheckCount();
    if (err != success) {
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
  error_code Simulator::initSim() {
    error_code err = success;
    int num_particles = 0;
    double ppm        = 0.0;

    err = init_file_.readHeader(ppm, num_particles);
    if (err != success) { return (err); }

    std::vector<Particle> particles = init_file_.readParticles();
    grid_.emplace(num_particles, ppm, particles);
    return (err);
  }

  /**
   * Representa la tercera fase de la simulacion, donde se procesan los calculos necesarios tantas
   * veces como el usuario indico por argumento
   * @return
   */
  error_code Simulator::process() {
    for (int i = 0; i < nts_; i++) {
      if (i > 0) {
        grid_->repositioning();
      }
      grid_->calculateAccelerations();
      grid_->processCollisions();
      grid_->moveParticles();
      grid_->processLimits();
    }
    return (success);
  }

  /**
   * Representa la ultima fase de la simulacion, se almacenan los resultados en el fichero
   * especificado por parametro
   * @return
   */
  error_code Simulator::storeResults() {
    int const num_particles = grid_->numParticles();
    std::vector<Particle const*> results(num_particles);

    final_file_.writeHeader(num_particles, grid_->particlesPerMeter());

    for (auto & block : grid_->getBlocks()) {
      for (auto & particle : block.particles) { results[particle.id] = &particle; }
    }

    final_file_.writeParticles(results);
    return (success);
  }

}  // namespace sim
