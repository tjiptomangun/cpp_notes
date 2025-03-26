#!/bin/bash
echo "no_memset" && date && ./no_memset && date
echo "with_memset" && date && ./with_memset && date
echo "first_only" && date && ./first_only && date
