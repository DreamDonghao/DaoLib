package main

import (
	"bytes"
	"encoding/json"
	"fmt"
	"log"
	"net/http"
	"os"
	"time"

	"github.com/robfig/cron/v3"
	"gopkg.in/yaml.v3"
)

// Config 配置结构
type Config struct {
	Bilibili struct {
		RegionID int `yaml:"region_id"` // 分区ID，0为全站热门
		Count    int `yaml:"count"`     // 获取数量
	} `yaml:"bilibili"`
	Server struct {
		URL     string `yaml:"url"`     // 目标服务器URL
		Method  string `yaml:"method"`  // HTTP方法
		Timeout int    `yaml:"timeout"` // 超时时间(秒)
	} `yaml:"server"`
	Schedule struct {
		Cron string `yaml:"cron"` // 定时表达式，默认每天9点: "0 9 * * *"
	} `yaml:"schedule"`
}

// VideoInfo B站视频信息
type VideoInfo struct {
	Aid       int64  `json:"aid"`
	Bvid      string `json:"bvid"`
	Title     string `json:"title"`
	Desc      string `json:"desc"`
	Owner     Owner  `json:"owner"`
	Stat      Stat   `json:"stat"`
	ShareURL  string `json:"share_url"`
	Duration  int    `json:"duration"`
	PublishAt int64  `json:"pubdate"`
}

type Owner struct {
	Mid  int64  `json:"mid"`
	Name string `json:"name"`
}

type Stat struct {
	View     int `json:"view"`
	Like     int `json:"like"`
	Coin     int `json:"coin"`
	Share    int `json:"share"`
	Danmaku  int `json:"danmaku"`
	Favorite int `json:"favorite"`
}

// BilibiliResponse B站热门API响应
type BilibiliResponse struct {
	Code    int `json:"code"`
	Message string `json:"message"`
	Data    struct {
		List []struct {
			Aid      int64  `json:"aid"`
			Bvid     string `json:"bvid"`
			Title    string `json:"title"`
			Desc     string `json:"desc"`
			Owner    Owner  `json:"owner"`
			Stat     Stat   `json:"stat"`
			ShareURL string `json:"share_url"`
			Duration int    `json:"duration"`
			Pubdate  int64  `json:"pubdate"`
		} `json:"list"`
	} `json:"data"`
}

// PushRequest 推送请求结构
type PushRequest struct {
	Bvid       string `json:"bvid"`
	Title      string `json:"title"`
	URL        string `json:"url"`
	Author     string `json:"author"`
	Duration   int    `json:"duration"`
	ViewCount  int    `json:"view_count"`
	LikeCount  int    `json:"like_count"`
	ShareCount int    `json:"share_count"`
	RecommendAt string `json:"recommend_at"`
}

var config Config

func main() {
	// 加载配置
	if err := loadConfig("config.yaml"); err != nil {
		log.Fatalf("加载配置失败: %v", err)
	}

	// 创建定时任务
	c := cron.New()

	// 添加定时任务
	_, err := c.AddFunc(config.Schedule.Cron, func() {
		log.Println("开始执行定时推送任务...")
		if err := pushVideo(); err != nil {
			log.Printf("推送失败: %v", err)
		} else {
			log.Println("推送成功")
		}
	})
	if err != nil {
		log.Fatalf("添加定时任务失败: %v", err)
	}

	c.Start()
	log.Printf("定时任务已启动，cron表达式: %s", config.Schedule.Cron)

	// 立即执行一次（可选）
	log.Println("立即执行一次推送...")
	if err := pushVideo(); err != nil {
		log.Printf("推送失败: %v", err)
	}

	// 阻塞主线程
	select {}
}

// loadConfig 加载配置文件
func loadConfig(path string) error {
	data, err := os.ReadFile(path)
	if err != nil {
		return fmt.Errorf("读取配置文件失败: %w", err)
	}

	// 设置默认值
	config.Bilibili.RegionID = 0
	config.Bilibili.Count = 10
	config.Server.Method = "POST"
	config.Server.Timeout = 30
	config.Schedule.Cron = "0 9 * * *" // 默认每天9点

	if err := yaml.Unmarshal(data, &config); err != nil {
		return fmt.Errorf("解析配置文件失败: %w", err)
	}

	return nil
}

// getHotVideos 获取B站热门视频
func getHotVideos() ([]VideoInfo, error) {
	// B站热门视频API
	url := fmt.Sprintf("https://api.bilibili.com/x/web-interface/popular?ps=%d&pn=1", config.Bilibili.Count)

	client := &http.Client{Timeout: 10 * time.Second}
	req, err := http.NewRequest("GET", url, nil)
	if err != nil {
		return nil, err
	}

	// 设置请求头，模拟浏览器
	req.Header.Set("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36")
	req.Header.Set("Referer", "https://www.bilibili.com")

	resp, err := client.Do(req)
	if err != nil {
		return nil, fmt.Errorf("请求B站API失败: %w", err)
	}
	defer resp.Body.Close()

	var bilibiliResp BilibiliResponse
	if err := json.NewDecoder(resp.Body).Decode(&bilibiliResp); err != nil {
		return nil, fmt.Errorf("解析响应失败: %w", err)
	}

	if bilibiliResp.Code != 0 {
		return nil, fmt.Errorf("B站API返回错误: %s", bilibiliResp.Message)
	}

	var videos []VideoInfo
	for _, item := range bilibiliResp.Data.List {
		videos = append(videos, VideoInfo{
			Aid:      item.Aid,
			Bvid:     item.Bvid,
			Title:    item.Title,
			Desc:     item.Desc,
			Owner:    item.Owner,
			Stat:     item.Stat,
			ShareURL: item.ShareURL,
			Duration: item.Duration,
		})
	}

	return videos, nil
}

// pushVideo 获取并推送视频
func pushVideo() error {
	// 获取热门视频
	videos, err := getHotVideos()
	if err != nil {
		return fmt.Errorf("获取热门视频失败: %w", err)
	}

	if len(videos) == 0 {
		return fmt.Errorf("没有获取到视频")
	}

	// 选择第一个视频推送（也可以随机选择）
	video := videos[0]

	// 构建推送数据
	pushData := PushRequest{
		Bvid:        video.Bvid,
		Title:       video.Title,
		URL:         video.ShareURL,
		Author:      video.Owner.Name,
		Duration:    video.Duration,
		ViewCount:   video.Stat.View,
		LikeCount:   video.Stat.Like,
		ShareCount:  video.Stat.Share,
		RecommendAt: time.Now().Format("2006-01-02 15:04:05"),
	}

	// 发送到目标服务器
	if err := sendToServer(pushData); err != nil {
		return fmt.Errorf("发送到服务器失败: %w", err)
	}

	log.Printf("已推送视频: [%s] %s - %s", video.Bvid, video.Title, video.Owner.Name)
	return nil
}

// sendToServer 发送数据到目标服务器
func sendToServer(data PushRequest) error {
	jsonData, err := json.Marshal(data)
	if err != nil {
		return fmt.Errorf("序列化数据失败: %w", err)
	}

	client := &http.Client{
		Timeout: time.Duration(config.Server.Timeout) * time.Second,
	}

	var req *http.Request
	var resp *http.Response

	if config.Server.Method == "GET" {
		req, err = http.NewRequest("GET", config.Server.URL, nil)
	} else {
		req, err = http.NewRequest("POST", config.Server.URL, bytes.NewBuffer(jsonData))
		req.Header.Set("Content-Type", "application/json")
	}

	if err != nil {
		return fmt.Errorf("创建请求失败: %w", err)
	}

	resp, err = client.Do(req)
	if err != nil {
		return fmt.Errorf("请求失败: %w", err)
	}
	defer resp.Body.Close()

	if resp.StatusCode >= 400 {
		return fmt.Errorf("服务器返回错误状态码: %d", resp.StatusCode)
	}

	return nil
}