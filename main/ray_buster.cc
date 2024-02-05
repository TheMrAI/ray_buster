#include "main/render/linear_partition.h"
#include "main/scenes/collection/cornell_box.h"

auto main() -> int
{
  render::linearPartition(scene::cornellBox());

  return 0;
}