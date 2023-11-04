# Webserv
# GitHub 협업

# 저장소(Repository)

---

<aside>
💡 ************저장소(Repository)************

- **원본 저장소 `upstream`**: 회사 또는 팀 소유의 저장소 (ex. ‣)
- **원격 저장소 `origin`**: 개인 소유의 원격 저장소 (ex. [github](https://github.com/))
- **로컬 저장소 `local`**: 개인 컴퓨터에 저장된 데이터베이스 (ex. `.git`)

![Untitled](GitHub%20%E1%84%92%E1%85%A7%E1%86%B8%E1%84%8B%E1%85%A5%E1%86%B8%201c069559ed1449e382ce7e7fb17fb522/Untitled.png)

</aside>

## 초기 설정

> **원본 저장소** → `fork`→ **원격 저장소** → `clone`→ **로컬 저장소**
> 

```bash
# 로컬 저장소 생성 
$ git clone git@github.com:사용자명/**원격저장소**.git
$ git remote -v
origin  git@github.com:사용자명/**원격저장소**.git (fetch)
origin  git@github.com:사용자명/**원격저장소**.git (push)
```

```bash
# 원본 저장소 연결
$ git remote add upstream git@github.com:조직명/**원본저장소**.git
$ git remote -v
origin  git@github.com:사용자명/**원격저장소**.git (fetch)
origin  git@github.com:사용자명/**원격저장소**.git (push)
upstream        git@github.com:조직명/**원본저장소**.git (fetch)
upstream        git@github.com:조직명/**원본저장소**.git (push)
```

## 동기화

> **원본 저장소** → `pull`→ **로컬 저장소** → `push` ****→ **원격 저장소** → `pull request`→ **원본 저장소**
> 

```bash
$ git pull upstream --rebase
	... 코드 변경 ... 
$ git push origin
# pull request 요청 
```

# 브랜치 (Branch)

---

<aside>
💡 **통합 브랜치(Integration Branch)** : 안정된 버전

- `master` 또는 `main`: 배포 버전
- `develop`: 개발 기능 병합

**토픽 브랜치(Topic Branch)** : 기능 추가 및 버그 수정 등 작업을 위한 버전

- `release`: 배포 준비
    - 형식 : `release-*` 또는 `release-RB_*` 또는 `release/*`
    
    ```bash
    $ git checkout -b relase-1.2 develop     # v1.2 브랜치 분기
    	# 배포 준비 완료
    $ git checkout master                    # 준비 완료 후 main으로 이동
    $ git merge --no-ff release-1.2          # main으로 브랜치 병합
    $ git tag -a 1.2                         # 병합한 커밋에 Release 버전 태그 부여
    $ git checkout develop                   # 준비 완료 후 develop으로 이동 
    $ git merge --no-ff release-1.2          # develop으로 브랜치 병합
    $ git branch -d release-1.2              # release-1.2 브랜치 제거
    ```
    
- `hotfix`: 긴급 패치
    - 형식 : `hotfix-*`
    
    ```bash
    $ git checkout -b hotfix-1.2.1 main      # v1.2.1 브랜치 분기
    	# 긴급 수정
    $ git checkout main                      # 준비 완료 후 main으로 이동
    $ git merge --no-ff hotfix-1.2.1         # main으로 브랜치 병합
    $ git tag -a 1.2.1                       # 병합한 커밋에 태그 부여
    $ git checkout develop                   # 준비 완료 후 develop으로 이동 
    $ git merge --no-ff release-1.2          # develop으로 브랜치 병합
    ```
    
- `feature`: 기능 개발
    - 형식 : `feature/<기능 요약>`
    
    ```bash
    $ git checkout -b feature/login develop   # login 기능 브랜치 분기
    	# 개발 완료
    $ git checkout develop                    # 개발 완료 후 develop으로 이동
    $ git merge --no-ff feature/login         # develop으로 브랜치 병합
    $ git branch -d feature/login             # login 브랜치 제거
    $ git push origin develop                 # 원격 저장소에 업로드
    ```
    

![[성공적인 Git 브랜칭 모델] [http://nvie.com/posts/a-successful-git-branching-model/](http://nvie.com/posts/a-successful-git-branching-model/) ](GitHub%20%E1%84%92%E1%85%A7%E1%86%B8%E1%84%8B%E1%85%A5%E1%86%B8%201c069559ed1449e382ce7e7fb17fb522/Untitled%201.png)

[성공적인 Git 브랜칭 모델] [http://nvie.com/posts/a-successful-git-branching-model/](http://nvie.com/posts/a-successful-git-branching-model/) 

</aside>

1. 무슨 작업을 할 건지 github에 이슈를 등록한다.
2. 브랜치를 생성한다.
    
    ```bash
    (develop)$ git checkout -b feature/login
    (feature/login)$
    ```
    
3. 원본 저장소의 최신 상태를 가져온 후 작업 내역에 대해 커밋한다.
    
    ```bash
    # 커밋 로그에 작성할 작업이 명확한 경우 
    (feature/login)$ git pull upstream --rebase  # 원본 저장소와 최신 상태 가져오기
    		# 충돌 발생 시 코드 수정 필요!
    (feature/login)$ git status                  # 상태 확인
    (feature/login)$ git add add_file            # untracked 파일 등록
    (feature/login)$ git diff modified_file      # modified 파일 변경사항 확인
    (feature/login)$ git status                  # 상태 확인
    (feature/login)$ git commit .                # 현재 디렉토리 커밋
    ```
    
    ```bash
    # 임시로 커밋해야하는 경우 (ex. 컴퓨터)
    (feature/login)$ git branch -b feature/login_temp
    (feature/login_temp)$ git add example.txt
    (feature/login_temp)$ git commit example.txt
    (feature/login_temp)$ git push
    
    # 임시 커밋을 가져오고 삭제 (ex. 노트북)
    (feature/login)$ git pull origin/featulre/login_temp
    (feature/login)$ git checkout feature/login_temp -- example.txt
    (feature/login)$ git branch -d feature/login_temp
    (feature/login)$ git push origin/featulre/login_temp
    ```
    
4. 로그를 확인 한 뒤 원격 저장소에 반영한다.
    
    ```bash
    # 로그 확인 
    (feature/login)$ git log --oneline -4
    611225a (HEAD -> develop_temp) fix: 로그인 버그 수정
    fd55f6e 누락파일 추가
    7200b90 feat: 로그인 기능 추가
    85bf8fc (..., main, develop) ... 
    
    # 이력 합치기
    (feature/login)$ git rebase -i 85bf8fc
    	# 수정 전 기본값
    	1 pick 7200b90 fix: 로그인 버그 수정
    	2 pick fd55f6e 누락파일 추가           # fixup로 변경
    	3 pick 611225a feat: 로그인 기능 추가
    
    	# 수정 후 저장
    	1 pick 7200b90 fix: 로그인 버그 수정
    	3 fixup fd55f6e 누락파일 추가          # ┬ 두 커밋이 하나로 합쳐짐
    	2 pick 611225a feat: 로그인 기능 추가  # ┘
    	
    # 로그 확인 (변경 완료)
    (feature/login)$ git log --oneline -3
    7f89bb7 (HEAD -> develop_temp) fix: 로그인 버그 수정
    eb6aca5 feat: 로그인 기능 추가
    85bf8fc (..., main, develop) ... 
    
    # 원격 저장소 반영
    (feature/login)$ git push
    ```
    
5. 원격 저장소의 내용을 원본 저장소에 `pull request`로 요청한다.
