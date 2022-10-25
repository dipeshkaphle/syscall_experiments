package main

import (
	"fmt"
	"os/exec"
	"strconv"
	"sync"
)

func main() {
	var wg sync.WaitGroup
	var mt sync.Mutex
	counter := 0
	C := 500
	n := 1000
	exec_chan := make(chan int, C)
	for j := 0; j < C; j++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			for {
				val := <-exec_chan

				if val == -1 {
					break
				}
				cmd := exec.Command("python", "controller.py", strconv.Itoa(val))
				if err := cmd.Run(); err != nil {
					fmt.Println(err.Error())
					mt.Lock()
					counter += 1
					mt.Unlock()
				}
			}
		}()
	}
	for i := 0; i < n; i++ {
		j := i
		exec_chan <- j
	}
	for j := 0; j < C; j++ {
		exec_chan <- -1
	}
	wg.Wait()
	fmt.Printf("Failures: %d, Total: %d", counter, n)
}
