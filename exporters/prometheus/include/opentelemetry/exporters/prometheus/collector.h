// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <memory>
#include <mutex>
#include <vector>

#include <prometheus/collectable.h>
#include <prometheus/metric_family.h>
#include "opentelemetry/exporters/prometheus/exporter_utils.h"

namespace prometheus_client = ::prometheus;

OPENTELEMETRY_BEGIN_NAMESPACE
namespace exporter
{
namespace metrics
{
/**
 * The Prometheus Collector maintains the intermediate collection in Prometheus Exporter
 */
class PrometheusCollector : public prometheus_client::Collectable
{
public:
  /**
   * Default Constructor.
   *
   * This constructor initializes the collection for metrics to export
   * in this class with default capacity
   */
  explicit PrometheusCollector(size_t max_collection_size = 2048);

  /**
   * Collects all metrics data from metricsToCollect collection.
   *
   * @return all metrics in the metricsToCollect snapshot
   */
  std::vector<prometheus_client::MetricFamily> Collect() const override;

  /**
   * This function is called by export() function and add the collection of
   * records to the metricsToCollect collection
   *
   * @param records a collection of records to add to the metricsToCollect collection
   */
  void AddMetricData(const sdk::metrics::ResourceMetrics &data);

  /**
   * Get the current collection in the collector.
   *
   * @return the current metricsToCollect collection
   */
  std::vector<std::unique_ptr<sdk::metrics::ResourceMetrics>> &GetCollection();

  /**
   * Gets the maximum size of the collection.
   *
   * @return max collection size
   */
  int GetMaxCollectionSize() const;

private:
  /**
   * Collection of metrics data from the export() function, and to be export
   * to user when they send a pull request. This collection is a pointer
   * to a collection so Collect() is able to clear the collection, even
   * though it is a const function.
   */
  mutable std::vector<std::unique_ptr<sdk::metrics::ResourceMetrics>> metrics_to_collect_;

  /**
   * Maximum size of the metricsToCollect collection.
   */
  size_t max_collection_size_;

  /*
   * Lock when operating the metricsToCollect collection
   */
  mutable std::mutex collection_lock_;
};
}  // namespace metrics
}  // namespace exporter
OPENTELEMETRY_END_NAMESPACE
